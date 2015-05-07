% Generated code: DO NOT EDIT MANUALLY
% For use in generating Matlab/Simulink + TrueTime
% models from ESMoL models.  Please consult ESMoL
% documentation for additional information.


VCP_DIR = getenv('VCP_PATH');
if strcmp(VCP_DIR, '')
  VCP_DIR = winqueryreg('HKEY_LOCAL_MACHINE', 'Software\ESMoL', 'VCP_PATH');
end
addpath( [VCP_DIR,'\bin'] );

init_ttenv;

% Initialize TrueTime
init_truetime;
% Make the simulink library available
load_system('simulink');
% Make the truetime library available
load_system('truetime');
% Create a new simulink model for the system
new_system('{{SYSTEMNAME}}');
% Open the new model
open_system('{{SYSTEMNAME}}');
% Set the simulation time to 2000
set_param('{{SYSTEMNAME}}','StopTime', '40');

% Create a new subsystem to put everything in
subsys_path_str = '{{SYSTEMNAME}}/{{SYSTEMNAME}}_subsys';
add_block( 'built-in/Subsystem', subsys_path_str );

{{#NETWORK_SECTION}}{{SYSTEMNAME}}_gen_{{BUSNAME}}( '{{SYSTEMNAME}}/{{SYSTEMNAME}}_subsys' ); 
{{/NETWORK_SECTION}}

{{#TTKERNEL_SECTION}}{{SYSTEMNAME}}_gen_{{KERNELNAME}}( '{{SYSTEMNAME}}/{{SYSTEMNAME}}_subsys' );
{{/TTKERNEL_SECTION}}

% Save and close the system
save_system('{{SYSTEMNAME}}');
close_system('{{SYSTEMNAME}}');
