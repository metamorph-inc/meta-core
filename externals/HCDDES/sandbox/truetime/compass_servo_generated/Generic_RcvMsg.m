function [exectime, data] = Generic_RcvMsg(segment, data)

switch segment,
    case 1,
        found = false;
        task_name = [data.node_name, '_task_', data.task_name];
        outp = ttGetMsg();
        % make sure this doesn't loop indefinitely - perhaps because it
        % only waits for a finite amount of time..
        while(~isempty(outp))
           if(~strcmp(outp.header, data.msg_name))
               outp = ttGetMsg();
           else
               found = true;
               break;
           end
        end
        
        if(found)
            t_data = ttGetData(task_name);
            t_data.input_data = outp.data;
            ttSetData(task_name, t_data);
        end
        
        % calculate the wcet:
        wcet = data.length / 100000;

        exectime = wcet;
    case 2,
        exectime = -1;
end

