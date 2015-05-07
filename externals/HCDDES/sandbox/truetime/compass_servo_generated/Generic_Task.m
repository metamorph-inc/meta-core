function [exectime, data] = Generic_Task(segment, data)

switch segment,
    case 1,
% getting something from local mem after data recieved from sender
% need to make sure that this gets kept locally constantly
% may want to name the actual task data to include specific sent data
% names... this could be generated higher up the chain.
        if(data.num_analog_inputs > 0)
            data.input_data = ttAnalogIn(1);
        end

        % only call if there is data that has been received...
        if(data.input_data)
            data.output_data = ttCallBlockSystem(1, data.input_data, [data.node_name '_fn']);
        end
        
        % set the number of outputs
        if(data.num_analog_outputs > 0)
            if(data.output_data)
                ttAnalogOut(1, data.output_data); % output the local data to the next (non-networked) object
            end
        end
        
        exectime = data.wcet;
    case 2,
        exectime = -1;
end

