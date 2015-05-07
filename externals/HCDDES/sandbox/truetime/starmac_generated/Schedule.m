%% Matlab struct - schedule - generated from TrueTime_CodeGen on 2008.10.11 - 12:31:22

schedule.name = 'StarMac_Model';


schedule.NetworkParameters(1).network_type = 'TDMA';
schedule.NetworkParameters(1).network_number = '1';
schedule.NetworkParameters(1).num_nodes = '2';
schedule.NetworkParameters(1).data_rate = '100000';
schedule.NetworkParameters(1).min_frame_size = '4096';
schedule.NetworkParameters(1).loss_prob = '0';
schedule.NetworkParameters(1).bandwidth_alloc = '[0.5 0.5]';
schedule.NetworkParameters(1).slotsize = '4096';
schedule.NetworkParameters(1).cyclic_sched = '[1 2]';
schedule.NetworkParameters(1).total_switch_mem = '671088640';


schedule.Nodes(1).name = 'Robostix';
schedule.Nodes(2).name = 'Gumstix';

schedule.hyperperiod  = 0;

schedule.Robostix.num_analog_inp = 0;
schedule.Robostix.num_analog_outp = 0;
schedule.Robostix.node_num = 1;
schedule.Robostix.task_list(1).task_name = 'inner_loop';
schedule.Robostix.task_list(1).start = 0;
schedule.Robostix.task_list(1).wcet = 0.012;
schedule.Robostix.msg_sent_list(1).msg_name = 'ThrustMsg';
schedule.Robostix.msg_sent_list(1).task_name = 'inner_loop';
schedule.Robostix.msg_sent_list(1).send_time = 0.012;
schedule.Robostix.msg_sent_list(1).length = 192;
schedule.Robostix.msg_sent_list(2).msg_name = 'StateMsg';
schedule.Robostix.msg_sent_list(2).task_name = 'inner_loop';
schedule.Robostix.msg_sent_list(2).send_time = 0.012;
schedule.Robostix.msg_sent_list(2).length = 432;
schedule.Robostix.msg_rcv_list(1).msg_name = 'ControlMsg';
schedule.Robostix.msg_rcv_list(1).task_name = 'inner_loop';
schedule.Robostix.msg_rcv_list(1).rcv_time = 0;
schedule.Robostix.msg_rcv_list(1).length = 192;
schedule.Robostix.msg_rcv_list(2).msg_name = 'SensorMsg';
schedule.Robostix.msg_rcv_list(2).task_name = 'inner_loop';
schedule.Robostix.msg_rcv_list(2).rcv_time = 0;
schedule.Robostix.msg_rcv_list(2).length = 432;


schedule.Gumstix.num_analog_inp = 0;
schedule.Gumstix.num_analog_outp = 0;
schedule.Gumstix.node_num = 2;
schedule.Gumstix.task_list(1).task_name = 'outer_loop';
schedule.Gumstix.task_list(1).start = 0.012;
schedule.Gumstix.task_list(1).wcet = 0.012;
schedule.Gumstix.msg_sent_list(1).msg_name = 'ControlMsg';
schedule.Gumstix.msg_sent_list(1).task_name = 'outer_loop';
schedule.Gumstix.msg_sent_list(1).send_time = 0.024;
schedule.Gumstix.msg_sent_list(1).length = 192;
schedule.Gumstix.msg_rcv_list(1).msg_name = 'RefMsg';
schedule.Gumstix.msg_rcv_list(1).task_name = 'outer_loop';
schedule.Gumstix.msg_rcv_list(1).rcv_time = 0.012;
schedule.Gumstix.msg_rcv_list(1).length = 144;
schedule.Gumstix.msg_rcv_list(2).msg_name = 'StateMsg';
schedule.Gumstix.msg_rcv_list(2).task_name = 'outer_loop';
schedule.Gumstix.msg_rcv_list(2).rcv_time = 0.012;
schedule.Gumstix.msg_rcv_list(2).length = 432;

