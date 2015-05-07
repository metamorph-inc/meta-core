function [exectime, data] = Generic_SendMsg(segment, data)

switch segment,
    case 1,
        task_name = [data.node_name, '_task_', data.task_name];
        outp = ttGetData(task_name);
        sent_data.header = data.msg_name;
        sent_data.data = outp.output_data;
        ttSendMsg(0, sent_data, 2000);
        
        % calculate the wcet:
        wcet = data.length / 100000;
        
        exectime = wcet;
    case 2,
        exectime = -1;
end

