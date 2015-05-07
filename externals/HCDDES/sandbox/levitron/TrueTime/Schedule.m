%% Matlab struct - schedule - generated from TrueTime_CodeGen on 2008.10.12 - 21:20:08

schedule.name = 'Levitron_Model';


schedule.NetworkParameters(1).network_type = 'TDMA';
schedule.NetworkParameters(1).network_number = '1';
schedule.NetworkParameters(1).num_nodes = '2';
schedule.NetworkParameters(1).data_rate = '100000';
schedule.NetworkParameters(1).min_frame_size = '8';
schedule.NetworkParameters(1).loss_prob = '0';
schedule.NetworkParameters(1).bandwidth_alloc = '[0.5 0.5]';
schedule.NetworkParameters(1).slotsize = '8';
schedule.NetworkParameters(1).cyclic_sched = '[1 2]';
schedule.NetworkParameters(1).total_switch_mem = '671088640';


schedule.Nodes(1).name = 'Gumstix';
schedule.Nodes(2).name = 'Robostix';

schedule.hyperperiod  = 0.02;

schedule.Gumstix.num_analog_inp = 0;
schedule.Gumstix.num_analog_outp = 0;
schedule.Gumstix.node_num = 1;
schedule.Gumstix.task_list(1).task_name = 'NavTask';
schedule.Gumstix.task_list(1).start = 0;
schedule.Gumstix.task_list(1).wcet = 0.02;
schedule.Gumstix.msg_sent_list(1).msg_name = 'NavMessage';
schedule.Gumstix.msg_sent_list(1).task_name = 'NavTask';
schedule.Gumstix.msg_sent_list(1).send_time = 0.02;
schedule.Gumstix.msg_sent_list(1).length = 32;
schedule.Gumstix.msg_rcv_list = [];


schedule.Robostix.num_analog_inp = 0;
schedule.Robostix.num_analog_outp = 0;
schedule.Robostix.node_num = 2;
schedule.Robostix.task_list(1).task_name = 'StabTask';
schedule.Robostix.task_list(1).start = 0.02;
schedule.Robostix.task_list(1).wcet = 0.005;
schedule.Robostix.msg_sent_list(1).msg_name = 'ForceMessage';
schedule.Robostix.msg_sent_list(1).task_name = 'StabTask';
schedule.Robostix.msg_sent_list(1).send_time = 0.025;
schedule.Robostix.msg_sent_list(1).length = 32;
schedule.Robostix.msg_rcv_list(1).msg_name = 'NavMessage';
schedule.Robostix.msg_rcv_list(1).task_name = 'StabTask';
schedule.Robostix.msg_rcv_list(1).rcv_time = 0.02;
schedule.Robostix.msg_rcv_list(1).length = 32;
schedule.Robostix.msg_rcv_list(2).msg_name = 'PosMessage';
schedule.Robostix.msg_rcv_list(2).task_name = 'StabTask';
schedule.Robostix.msg_rcv_list(2).rcv_time = 0.02;
schedule.Robostix.msg_rcv_list(2).length = 32;

