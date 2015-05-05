import sys
import os
from common import PostProcess, update_metrics_in_report_json
from common import read_limits, check_limits_and_add_to_report_json


def main():
    mat_file_name = sys.argv[1]
    if not os.path.exists(mat_file_name):
        raise IOError('Given result file does not exist: {0}'.format(sys.argv[1]))

    ## First limit part
    limit_dict, filter = read_limits()
    ## End of first limit part

    ## Post processing part
    bucketCylLength_uri = 'Excavator_.bucketCylLength_unit'
    filter.append(bucketCylLength_uri)
    armCylLength_uri = 'Excavator_.armCylLength_unit'
    filter.append(armCylLength_uri)
    boomCylLength_uri = 'Excavator_.boomCylLength_unit'
    filter.append(boomCylLength_uri)
    boomCylRPressure_uri = 'Excavator_.boomCylLPressure_a'
    filter.append(boomCylRPressure_uri)    
    armCylPressure_uri = 'Excavator_.armCylPressure_a'
    filter.append(armCylPressure_uri)
    bucketCylPressure_uri = 'Excavator_.bucketCylPressure_a'
    filter.append(bucketCylPressure_uri)
    arm_ang_vel_uri = 'Excavator_.arm_ang_vel'
    filter.append(arm_ang_vel_uri)
    max_Y_uri = 'Excavator_.yDistance'
    filter.append(max_Y_uri)
    max_reach_uri = 'Excavator_.xDistance'
    filter.append(max_reach_uri)
    swing_uri = 'Excavator_.carriage2.swingRevolute.w'
    filter.append(swing_uri)

    # loads results with the filtered out variables (and 'time' which is default)
    pp = PostProcess(mat_file_name, filter)
    
    max_p_time = pp.global_max_time(boomCylRPressure_uri)
    max_v_time = pp.global_max_time(arm_ang_vel_uri)
    print 'Maximum pressure obtained at : {0}'.format(max_p_time)
    max_p_time = pp.global_max_time(armCylPressure_uri)
    print 'Maximum pressure obtained at : {0}'.format(max_p_time)
    max_p_time = pp.global_max_time(bucketCylPressure_uri)
    print 'Maximum pressure obtained at : {0}'.format(max_p_time)
    print 'Maximum velocity obtained at : {0}'.format(max_v_time)
    print 'Maximum Reach obtained at : {0}'.format(pp.global_max_time(max_reach_uri))
    metrics = {}
    metrics.update({'bucketCylLength_unit': {'value': pp.get_data_by_time(bucketCylLength_uri, max_p_time)[0], 'unit': 'm'},
                    'boomCylLength_unit': {'value': pp.get_data_by_time(boomCylLength_uri, max_p_time)[0], 'unit': 'm'},
                    'boomCylRPressure': {'value': pp.global_abs_max(boomCylRPressure_uri)*0.00001, 'unit': 'bar'},
                    'armCylLength_unit': {'value': pp.get_data_by_time(armCylLength_uri, max_p_time)[0], 'unit': 'm'},
                    'armCylPressure': {'value': pp.global_abs_max(armCylPressure_uri)*0.00001, 'unit': 'bar'},
                    'bucketCylPressure': {'value': pp.global_abs_max(bucketCylPressure_uri)*0.00001, 'unit': 'bar'},
                    'arm_angVel' : {'value': pp.global_abs_max(arm_ang_vel_uri), 'unit':'rads/s'},
                    'max_reach':{'value':pp.global_max(max_reach_uri),'unit':'m'},
                    'max_high_reach' : {'value': pp.global_max(max_Y_uri), 'unit':'m'},
                    'max_low_reach': {'value': pp.global_min(max_Y_uri), 'unit':'m'},
                    'swing_speed': {'value': pp.last_value(swing_uri), 'unit':'rad/s'},
                    })

    cwd = os.getcwd()
    os.chdir('..')
    # print 'Plot saved to : {0}'.format(pp.save_as_svg(vehicle_speed,
                                                      # pp.global_abs_max(vehicle_speed),
                                                      # 'VehicleSpeed',
                                                      # 'max(FTP_Driver.driver_bus.vehicle_speed)',
                                                      # 'kph'))
    pp.store_data_to_csv(bucketCylLength_uri, '{0}.csv'.format(bucketCylLength_uri), 0, 0.1, 200)
    pp.store_data_to_csv(armCylLength_uri, '{0}.csv'.format(armCylLength_uri), 0, 0.1, 200)
    pp.store_data_to_csv(boomCylLength_uri, '{0}.csv'.format(boomCylLength_uri), 0, 0.1, 200)
    pp.store_data_to_csv(boomCylRPressure_uri, '{0}.csv'.format(boomCylRPressure_uri), 0, 0.1, 200)
    pp.store_data_to_csv(arm_ang_vel_uri, '{0}.csv'.format(arm_ang_vel_uri), 0, 0.1, 200)
    pp.store_data_to_csv(max_Y_uri, '{0}.csv'.format(max_Y_uri), 0, 0.1, 500)
    pp.store_data_to_csv(max_reach_uri, '{0}.csv'.format(max_reach_uri), 0, 0.1, 200)
    update_metrics_in_report_json(metrics)
    ## end of postprocessing part

    ## Second limit part
    check_limits_and_add_to_report_json(pp, limit_dict)
    ## end of Second limit part
    os.chdir(cwd)

if __name__ == '__main__':
    root_dir = os.getcwd()
    try:
        main()
    except:
        os.chdir(root_dir)
        import traceback
        trace = traceback.format_exc()
        # Generate this file on failed executions, https://github.com/scipy/scipy/issues/1840
        with open(os.path.join('..', '_POST_PROCESSING_FAILED.txt'), 'wb') as f_out:
            f_out.write(trace)