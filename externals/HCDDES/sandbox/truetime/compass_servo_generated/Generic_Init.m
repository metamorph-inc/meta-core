function Generic_Init(schedule)
% Additional Notes:
% create logs using the ttlogging commands

    % hard coded to remove the 'truetime kernel' part of the name
    temp = regexp(gcs, '(/TrueTime Kernel)', 'split');
    node_name = get_param(temp(1), 'Name');
    prio = 1;
    num_input_output = [schedule.(char(node_name)).num_analog_inp, schedule.(char(node_name)).num_analog_outp];
    
    % num inputs, num outputs, scheduling policy (fixed prio)
    ttInitKernel(num_input_output(1), num_input_output(2), 'prioFP');
    % Initialize network
    ttCreateInterruptHandler('nw_handler', prio, 'Generic_MsgDoNothing');
    ttInitNetwork(schedule.(char(node_name)).node_num, 'nw_handler'); % node # 'num' in the network
    
    CreateTasks(node_name, schedule.hyperperiod, schedule.(char(node_name)).task_list, num_input_output);
    CreateSentMsgs(node_name, schedule.hyperperiod, schedule.(char(node_name)).msg_sent_list);
    CreateRcvMsgs(node_name, schedule.hyperperiod, schedule.(char(node_name)).msg_rcv_list);
end

function CreateTasks(node_name, hyperperiod, task_data, num_input_output)
    % schedule the calls to this node, since it is known a priori
    prio = 1;
    num_tasks = size(task_data, 2);
    
    for i=1:num_tasks
        data = task_data(i);
        data.input_data = [];
        data.output_data = [];
        data.node_name = char(node_name);
        data.num_analog_inputs = num_input_output(1);
        data.num_analog_outputs = num_input_output(2);

        ttCreatePeriodicTask([char(node_name) '_task_' char(data.task_name)], data.start, hyperperiod, prio, 'Generic_Task', data);
    end
end

function CreateSentMsgs(node_name, hyperperiod, msg_data)
    prio = 1;
    num_msgs = size(msg_data, 2);
    
    for i=1:num_msgs
        data = msg_data(i);
        data.node_name = char(node_name);

        ttCreatePeriodicTask([char(node_name) '_sendmsg_' char(data.msg_name)], data.send_time, hyperperiod, prio, 'Generic_SendMsg', data);
    end
end

function CreateRcvMsgs(node_name, hyperperiod, msg_data)
    prio = 1;
    num_msgs = size(msg_data, 2);
    
    for i=1:num_msgs
        data = msg_data(i);
        data.node_name = char(node_name);

        ttCreatePeriodicTask([char(node_name) '_rcvmsg_' char(data.msg_name)], data.rcv_time, hyperperiod, prio, 'Generic_RcvMsg', data);
    end
end

