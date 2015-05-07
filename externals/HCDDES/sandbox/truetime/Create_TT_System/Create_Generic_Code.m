function [] = Create_Generic_Code()

% generate Generic_Init
fid = fopen('Generic_Init.m','w');

fprintf(fid,'%s\n',     'function Generic_Init(schedule)');
fprintf(fid,'%s\n',     '% Additional Notes:');
fprintf(fid,'%s\n',     '% create logs using the ttlogging commands');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '    % hard coded to remove the ''truetime kernel'' part of the name');
fprintf(fid,'%s\n',     '    temp = regexp(gcs, ''(/TrueTime Kernel)'', ''split'');');
fprintf(fid,'%s\n',     '    node_name = get_param(temp(1), ''Name'');');
fprintf(fid,'%s\n',     '    prio = 1;');
fprintf(fid,'%s\n',     '    num_input_output = [schedule.(char(node_name)).num_analog_inp, schedule.(char(node_name)).num_analog_outp];');
fprintf(fid,'%s\n',     '    ');
fprintf(fid,'%s\n',     '    % num inputs, num outputs, scheduling policy (fixed prio)');
fprintf(fid,'%s\n',     '    ttInitKernel(num_input_output(1), num_input_output(2), ''prioFP'');');
fprintf(fid,'%s\n',     '    % Initialize network');
fprintf(fid,'%s\n',     '    ttCreateInterruptHandler(''nw_handler'', prio, ''Generic_MsgDoNothing'');');
fprintf(fid,'%s\n',     '    ttInitNetwork(schedule.(char(node_name)).node_num, ''nw_handler''); % node # ''num'' in the network');
fprintf(fid,'%s\n',     '    ');
fprintf(fid,'%s\n',     '    CreateTasks(node_name, schedule.hyperperiod, schedule.(char(node_name)).task_list, num_input_output);');
fprintf(fid,'%s\n',     '    CreateSentMsgs(node_name, schedule.hyperperiod, schedule.(char(node_name)).msg_sent_list);');
fprintf(fid,'%s\n',     '    CreateRcvMsgs(node_name, schedule.hyperperiod, schedule.(char(node_name)).msg_rcv_list);');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'function CreateTasks(node_name, hyperperiod, task_data, num_input_output)');
fprintf(fid,'%s\n',     '    % schedule the calls to this node, since it is known a priori');
fprintf(fid,'%s\n',     '    prio = 1;');
fprintf(fid,'%s\n',     '    num_tasks = size(task_data, 2);');
fprintf(fid,'%s\n',     '    ');
fprintf(fid,'%s\n',     '    for i=1:num_tasks');
fprintf(fid,'%s\n',     '        data = task_data(i);');
fprintf(fid,'%s\n',     '        data.input_data = [];');
fprintf(fid,'%s\n',     '        data.output_data = [];');
fprintf(fid,'%s\n',     '        data.node_name = char(node_name);');
fprintf(fid,'%s\n',     '        data.num_analog_inputs = num_input_output(1);');
fprintf(fid,'%s\n',     '        data.num_analog_outputs = num_input_output(2);');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '        ttCreatePeriodicTask([char(node_name) ''_task_'' char(data.task_name)], data.start, hyperperiod, prio, ''Generic_Task'', data);');
fprintf(fid,'%s\n',     '    end');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'function CreateSentMsgs(node_name, hyperperiod, msg_data)');
fprintf(fid,'%s\n',     '    prio = 1;');
fprintf(fid,'%s\n',     '    num_msgs = size(msg_data, 2);');
fprintf(fid,'%s\n',     '    ');
fprintf(fid,'%s\n',     '    for i=1:num_msgs');
fprintf(fid,'%s\n',     '        data = msg_data(i);');
fprintf(fid,'%s\n',     '        data.node_name = char(node_name);');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '        ttCreatePeriodicTask([char(node_name) ''_sendmsg_'' char(data.msg_name)], data.send_time, hyperperiod, prio, ''Generic_SendMsg'', data);');
fprintf(fid,'%s\n',     '    end');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'function CreateRcvMsgs(node_name, hyperperiod, msg_data)');
fprintf(fid,'%s\n',     '    prio = 1;');
fprintf(fid,'%s\n',     '    num_msgs = size(msg_data, 2);');
fprintf(fid,'%s\n',     '    ');
fprintf(fid,'%s\n',     '    for i=1:num_msgs');
fprintf(fid,'%s\n',     '        data = msg_data(i);');
fprintf(fid,'%s\n',     '        data.node_name = char(node_name);');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '        ttCreatePeriodicTask([char(node_name) ''_rcvmsg_'' char(data.msg_name)], data.rcv_time, hyperperiod, prio, ''Generic_RcvMsg'', data);');
fprintf(fid,'%s\n',     '    end');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');

fclose(fid);

% generate Generic_MsgDoNothing
fid = fopen('Generic_MsgDoNothing.m','w');

fprintf(fid,'%s\n',     'function [exectime, data] = Generic_MsgDoNothing(seg, data)');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'exectime = -1;');
fprintf(fid,'%s\n',     '');

fclose(fid);

% generate Generic_RcvMsg
fid = fopen('Generic_RcvMsg.m','w');

fprintf(fid,'%s\n',     'function [exectime, data] = Generic_RcvMsg(segment, data)');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'switch segment,');
fprintf(fid,'%s\n',     '    case 1,');
fprintf(fid,'%s\n',     '        found = false;');
fprintf(fid,'%s\n',     '        task_name = [data.node_name, ''_task_'', data.task_name];');
fprintf(fid,'%s\n',     '        outp = ttGetMsg();');
fprintf(fid,'%s\n',     '        % make sure this doesn''t loop indefinitely - perhaps because it');
fprintf(fid,'%s\n',     '        % only waits for a finite amount of time..');
fprintf(fid,'%s\n',     '        while(~isempty(outp))');
fprintf(fid,'%s\n',     '           if(~strcmp(outp.header, data.msg_name))');
fprintf(fid,'%s\n',     '               outp = ttGetMsg();');
fprintf(fid,'%s\n',     '           else');
fprintf(fid,'%s\n',     '               found = true;');
fprintf(fid,'%s\n',     '               break;');
fprintf(fid,'%s\n',     '           end');
fprintf(fid,'%s\n',     '        end');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        if(found)');
fprintf(fid,'%s\n',     '            t_data = ttGetData(task_name);');
fprintf(fid,'%s\n',     '            t_data.input_data = outp.data;');
fprintf(fid,'%s\n',     '            ttSetData(task_name, t_data);');
fprintf(fid,'%s\n',     '        end');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        % calculate the wcet:');
fprintf(fid,'%s\n',     '        wcet = data.length / 100000;');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '        exectime = wcet;');
fprintf(fid,'%s\n',     '    case 2,');
fprintf(fid,'%s\n',     '        exectime = -1;');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');

fclose(fid);

% generate Generic_SendMsg
fid = fopen('Generic_SendMsg.m','w');

fprintf(fid,'%s\n',     'function [exectime, data] = Generic_SendMsg(segment, data)');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'switch segment,');
fprintf(fid,'%s\n',     '    case 1,');
fprintf(fid,'%s\n',     '        task_name = [data.node_name, ''_task_'', data.task_name];');
fprintf(fid,'%s\n',     '        outp = ttGetData(task_name);');
fprintf(fid,'%s\n',     '        sent_data.header = data.msg_name;');
fprintf(fid,'%s\n',     '        sent_data.data = outp.output_data;');
fprintf(fid,'%s\n',     '        ttSendMsg(0, sent_data, 2000);');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        % calculate the wcet:');
fprintf(fid,'%s\n',     '        wcet = data.length / 100000;');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        exectime = wcet;');
fprintf(fid,'%s\n',     '    case 2,');
fprintf(fid,'%s\n',     '        exectime = -1;');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');

fclose(fid);

% generate Generic_Task
fid = fopen('Generic_Task.m','w');

fprintf(fid,'%s\n',     'function [exectime, data] = Generic_Task(segment, data)');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     'switch segment,');
fprintf(fid,'%s\n',     '    case 1,');
fprintf(fid,'%s\n',     '% getting something from local mem after data recieved from sender');
fprintf(fid,'%s\n',     '% need to make sure that this gets kept locally constantly');
fprintf(fid,'%s\n',     '% may want to name the actual task data to include specific sent data');
fprintf(fid,'%s\n',     '% names... this could be generated higher up the chain.');
fprintf(fid,'%s\n',     '        if(data.num_analog_inputs > 0)');
fprintf(fid,'%s\n',     '            data.input_data = ttAnalogIn(1);');
fprintf(fid,'%s\n',     '        end');
fprintf(fid,'%s\n',     '');
fprintf(fid,'%s\n',     '        % only call if there is data that has been received...');
fprintf(fid,'%s\n',     '        if(data.input_data)');
fprintf(fid,'%s\n',     '            data.output_data = ttCallBlockSystem(1, data.input_data, [data.node_name ''_fn'']);');
fprintf(fid,'%s\n',     '        end');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        % set the number of outputs');
fprintf(fid,'%s\n',     '        if(data.num_analog_outputs > 0)');
fprintf(fid,'%s\n',     '            if(data.output_data)');
fprintf(fid,'%s\n',     '                ttAnalogOut(1, data.output_data); % output the local data to the next (non-networked) object');
fprintf(fid,'%s\n',     '            end');
fprintf(fid,'%s\n',     '        end');
fprintf(fid,'%s\n',     '        ');
fprintf(fid,'%s\n',     '        exectime = data.wcet;');
fprintf(fid,'%s\n',     '    case 2,');
fprintf(fid,'%s\n',     '        exectime = -1;');
fprintf(fid,'%s\n',     'end');
fprintf(fid,'%s\n',     '');

fclose(fid);

end