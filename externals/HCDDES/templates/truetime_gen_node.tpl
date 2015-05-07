{{#TTKERNEL_SECTION}}function [] = {{SYSTEMNAME}}_gen_{{KERNEL_NAME}}( subsys_path_str )
%
% function [] = truetime_gen_{{KERNEL_NAME}}( subsys_path_str )
%
%   Create the TrueTime kernel model within the given subsystem path

% Generated code: DO NOT EDIT MANUALLY
% For use in generating Matlab/Simulink + TrueTime
% models from ESMoL models.  Please consult ESMoL
% documentation for additional information.

% Add a TT Kernel block for node {{KERNEL_NAME}}
node_path_str = strcat( subsys_path_str, '/{{KERNEL_NAME}}' );
add_block('truetime/TrueTime Kernel', node_path_str);
set_param(node_path_str,'args','{{KERNEL_ARGS}}');
set_param(node_path_str,'sfun','{{KERNEL_INITFUNC}}');
set_param(node_path_str,'ninputsoutputs','{{KERNEL_IOPORTS}}');
set_param(node_path_str,'nwnodenbr','{{KERNEL_NETNODENUM}}');

% Add a scope to the scheduler output of the kernel block
node_scope_path_str = strcat( node_path_str, '_Sched' );
add_block('simulink/Sinks/Scope', node_scope_path_str );

% Connect the scheduler output to the scope
add_line(subsys_path_str,'{{KERNEL_NAME}}/{{KERNEL_SCOPE_PORT}}','{{KERNEL_NAME}}_Sched/1','autorouting','off');
{{#TTKERNEL_AD_SECTION}}
% Create mux for the A/D
node_mux_path_str = strcat( node_path_str, '_mux' );
add_block( 'simulink/Signal Routing/Mux', node_mux_path_str );
add_line( subsys_path_str, strcat( '{{KERNEL_NAME}}', '_mux', '/1' ), '{{KERNEL_NAME}}/{{KERNEL_AD_PORT}}' );
set_param( node_mux_path_str, 'Inputs', '{{KERNEL_AD_SIZE_VECTOR}}' );
{{#INPUT_PORT_SECTION}}
in_port_name = strcat( '{{INPUT_PORT_NAME}}', '_in_', '{{INPUT_PORT_NUM}}' );
in_port_path = strcat( subsys_path_str, '/', in_port_name );
add_block( 'built-in/Inport', in_port_path );
set_param( in_port_path, 'Port', '{{INPUT_PORT_NUM}}' );
add_line( subsys_path_str, strcat( in_port_name, '/1' ), strcat( '{{KERNEL_NAME}}', '_mux', '/', '{{INPUT_MUX_PORT_NUM}}' ) );
{{/INPUT_PORT_SECTION}}{{/TTKERNEL_AD_SECTION}}{{#TTKERNEL_DA_SECTION}}
% Create demux for the D/A
node_demux_path_str = strcat( node_path_str, '_demux' );
add_block( 'simulink/Signal Routing/Demux', node_demux_path_str );
add_line( subsys_path_str, '{{KERNEL_NAME}}/{{KERNEL_DA_PORT}}', strcat( '{{KERNEL_NAME}}', '_demux', '/1' ) );
set_param( node_demux_path_str, 'Outputs', '{{KERNEL_DA_SIZE_VECTOR}}' );
{{#OUTPUT_PORT_SECTION}}
out_port_name = strcat( '{{OUTPUT_PORT_NAME}}', '_out_', '{{OUTPUT_PORT_NUM}}' );
out_port_path = strcat( subsys_path_str, '/', out_port_name );
add_block( 'built-in/Outport', out_port_path );
set_param( out_port_path, 'Port', '{{OUTPUT_PORT_NUM}}' );
add_line( subsys_path_str, strcat( '{{KERNEL_NAME}}', '_demux', '/', '{{OUTPUT_PORT_NUM}}' ), strcat( out_port_name, '/1' ) );   
{{/OUTPUT_PORT_SECTION}}{{/TTKERNEL_DA_SECTION}}{{/TTKERNEL_SECTION}}
