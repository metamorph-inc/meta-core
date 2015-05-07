% this could be created through the input of the initial specs and the
% output schedule for everything
% execution times for the messages can be calculated from length else will
% just be static = .001 (assumption)

schedule.name = 'Compass_Servo_Model';

schedule.NetworkParameters(1).network_type = 'TDMA';
schedule.NetworkParameters(1).network_number = '1';
schedule.NetworkParameters(1).num_nodes = '4';
schedule.NetworkParameters(1).data_rate = '10000000';
schedule.NetworkParameters(1).min_frame_size = '512';
schedule.NetworkParameters(1).loss_prob = '0';
schedule.NetworkParameters(1).bandwidth_alloc = '[0.5 0.5]';
schedule.NetworkParameters(1).slotsize = '512';
schedule.NetworkParameters(1).cyclic_sched = '[1 2 3 4]';
schedule.NetworkParameters(1).total_switch_mem = '80000';

schedule.Nodes(1).name = 'CompassDriver';
schedule.Nodes(2).name = 'Smoothing';
schedule.Nodes(3).name = 'Scaler';
schedule.Nodes(4).name = 'ServoDriver';

schedule.hyperperiod = .125;

schedule.CompassDriver.num_analog_inp = 1;
schedule.CompassDriver.num_analog_outp = 0;
schedule.CompassDriver.node_num = 1;
schedule.CompassDriver.task_list(1).task_name = 'task1';
schedule.CompassDriver.task_list(1).start = 0;
schedule.CompassDriver.task_list(1).wcet = 0.02;
schedule.CompassDriver.msg_sent_list(1).msg_name = 'RawHeading';
schedule.CompassDriver.msg_sent_list(1).task_name = 'task1';
schedule.CompassDriver.msg_sent_list(1).send_time = 0.021;
schedule.CompassDriver.msg_sent_list(1).length = 2000;
schedule.CompassDriver.msg_rcv_list = [];

schedule.Smoothing.num_analog_inp = 0;
schedule.Smoothing.num_analog_outp = 0;
schedule.Smoothing.node_num = 2;
schedule.Smoothing.task_list(1).task_name = 'task1';
schedule.Smoothing.task_list(1).start = 0.025;
schedule.Smoothing.task_list(1).wcet = 0.045;
schedule.Smoothing.msg_sent_list(1).msg_name = 'Heading';
schedule.Smoothing.msg_sent_list(1).task_name = 'task1';
schedule.Smoothing.msg_sent_list(1).send_time = 0.071;
schedule.Smoothing.msg_sent_list(1).length = 2000;
schedule.Smoothing.msg_rcv_list(1).msg_name = 'RawHeading';
schedule.Smoothing.msg_rcv_list(1).task_name = 'task1';
schedule.Smoothing.msg_rcv_list(1).rcv_time = 0.023;
schedule.Smoothing.msg_rcv_list(1).length = 2000;

schedule.Scaler.num_analog_inp = 0;
schedule.Scaler.num_analog_outp = 0;
schedule.Scaler.node_num = 3;
schedule.Scaler.task_list(1).task_name = 'task1';
schedule.Scaler.task_list(1).start = 0.075;
schedule.Scaler.task_list(1).wcet = 0.02;
schedule.Scaler.msg_sent_list(1).msg_name = 'PulseWidth';
schedule.Scaler.msg_sent_list(1).task_name = 'task1';
schedule.Scaler.msg_sent_list(1).send_time = 0.096;
schedule.Scaler.msg_sent_list(1).length = 2000;
schedule.Scaler.msg_rcv_list(1).msg_name = 'Heading';
schedule.Scaler.msg_rcv_list(1).task_name = 'task1';
schedule.Scaler.msg_rcv_list(1).rcv_time = .073;
schedule.Scaler.msg_rcv_list(1).length = 2000;

schedule.ServoDriver.num_analog_inp = 0;
schedule.ServoDriver.num_analog_outp = 1;
schedule.ServoDriver.node_num = 4;
schedule.ServoDriver.task_list(1).task_name = 'task1';
schedule.ServoDriver.task_list(1).start = 0.1;
schedule.ServoDriver.task_list(1).wcet = 0.01;
schedule.ServoDriver.msg_sent_list = [];
schedule.ServoDriver.msg_rcv_list(1).msg_name = 'PulseWidth';
schedule.ServoDriver.msg_rcv_list(1).task_name = 'task1';
schedule.ServoDriver.msg_rcv_list(1).rcv_time = 0.098;
schedule.ServoDriver.msg_rcv_list(1).length = 2000;

% ***then look at what happens at different speeds hz/hyperperiod
% connect the output back to input and look at it as a trajectory
% controller - as increase the speed/decrease will see degradation in
% performance
