{{#NETWORK_SECTION}}function [] = {{SYSTEMNAME}}_gen_{{NETWORK_NAME}}( subsys_path_str )
%
% function [] = {{SYSTEMNAME}}_gen_{{NETWORK_NAME}}( subsys_path_str )
%
%   Create the TrueTime network model within the given subsystem path

% Generated code: DO NOT EDIT MANUALLY
% For use in generating Matlab/Simulink + TrueTime
% models from ESMoL models.  Please consult ESMoL
% documentation for additional information.

% Add a TT Network block for bus: {{NETWORK_NAME}}
network_path_str = strcat( subsys_path_str, '/{{NETWORK_NAME}}' );
add_block('truetime/TrueTime Network',network_path_str);
set_param(network_path_str,'nwtype','{{NETWORK_TYPE}}');
set_param(network_path_str,'nwnbr','{{NETWORK_NUMBER}}');
set_param(network_path_str,'nnodes','{{NETWORK_NUMNODES}}');
set_param(network_path_str,'rate','{{NETWORK_DATARATE}}');
set_param(network_path_str,'minsize','{{NETWORK_FRAMESIZE}}');
set_param(network_path_str,'lossprob','{{NETWORK_LOSSPROB}}');
set_param(network_path_str,'seed','{{NETWORK_SEED}}');
{{#TT_SECTION}}set_param(network_path_str,'schedule','{{NETWORK_SCHEDULE}}');{{/TT_SECTION}}
% Add a scope for scheduler output of the network block
network_scope_path_str = strcat( network_path_str, '_Sched' );
add_block('simulink/Sinks/Scope', network_scope_path_str );
% Finally, connect the scheduler output to the scope
add_line(subsys_path_str,'{{NETWORK_NAME}}/1','{{NETWORK_NAME}}_Sched/1','autorouting','off');
{{/NETWORK_SECTION}}
