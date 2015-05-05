import sys
import os
from common import PostProcess, update_metrics_in_report_json
from common import read_limits, check_limits_and_add_to_report_json
import numpy as np

def RMS(data, time, t_end):
    """
    Returns the integral of data at the time-interval of time.
    """
    sum_2 = 0
    next_data = data[0]
    next_t = time[0]
    ncp = 0
    for i in xrange(data.size):
        cur_data = next_data
        next_data = data[i]
        height = (next_data + cur_data) / 2
        
        cur_t = next_t
        next_t = time[i]
        interval = next_t - cur_t
        ds_2 = (height**2) * interval
        sum_2 += ds_2
    return np.sqrt(sum_2/t_end)

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
        filter.append('driver_Land_Profile.ModelicaModel_Driver_Land_Profile.driver_control.error_current.y')
        filter.append('driver_Land_Profile.ModelicaModel_Driver_Land_Profile.driver_control.error_current.u2')
        
        #filter.append('road_Wheel_Load_Both_Sides.ModelicaModel_road_Wheel_Load_Both_Sides.Accel_40kph')

        # loads results with the filtered out variables (and 'time' which is default)
        pp = PostProcess(mat_file_name, filter)
        t = pp.time
        # Liters of fuel used per 100km
        volume_var_name = [var_name for var_name in filter if var_name.endswith('tank.V')][0]
        volume = pp.data_array(volume_var_name)
        fuel_used = (volume[0] - volume[-1])*1000
        distance_covered_m = pp.integrate('driver_Land_Profile.ModelicaModel_Driver_Land_Profile.driver_control.error_current.u2')
        if distance_covered_m != 0:
            fuel_consumption = fuel_used/(distance_covered_m*1000*100)
        else:
            fuel_consumption = -1
        #vehicle_speed_SI = pp.data_array('driver_Land_Profile.ModelicaModel_Driver_Land_Profile.driver_control.error_current.u2')
        
        # Deviation from requested speed
        e = pp.data_array('driver_Land_Profile.ModelicaModel_Driver_Land_Profile.driver_control.error_current.y')
        
        index_neg = np.where(e < 0)[0]
        index_pos = np.where(e > 0)[0]
        
        rms_tot = RMS(e, t, t[-1])
        rms_e_neg = RMS(e[index_neg], t[index_neg], t[-1])
        rms_e_pos = RMS(e[index_pos], t[index_pos], t[-1])
        
        avg_response = 1 / (1 + rms_tot)
        acc_response = 1 / (1 + rms_e_neg)
        dec_response = 1 / (1 + rms_e_pos)
        
        metrics = {}
        #metrics.update({'VehicleSpeed': {'value': pp.global_abs_max("road_Wheel_Load_Both_Sides.ModelicaModel_road_Wheel_Load_Both_Sides.vehicleSpeed"), 'unit': 'kph'}})
        metrics.update({'DistanceCovered': {'value': distance_covered_m/1000, 'unit': 'km'}})
        metrics.update({'FuelConsumption': {'value': fuel_consumption, 'unit': 'liters/100km'}})
        
        metrics.update({'AccelerationResponse': {'value': acc_response, 'unit': ''}})
        metrics.update({'DecelerationResponse': {'value': dec_response, 'unit': ''}})
        metrics.update({'AverageResponse': {'value': avg_response, 'unit': ''}})
        
        #metrics.update({'Acc40kph': {'value': pp.last_value('road_Wheel_Load_Both_Sides.ModelicaModel_road_Wheel_Load_Both_Sides.Accel_40kph'), 'unit': 's'}})
        #metrics.update({'EngineAirPressure': {'value': pp.last_value(pressure_variable_name), 'unit': 'Pascal'}})

        update_metrics_in_report_json(metrics)
        ## end of postprocessing part

        ## Second limit part
        check_limits_and_add_to_report_json(pp, limit_dict)
        ## end of Second limit part


