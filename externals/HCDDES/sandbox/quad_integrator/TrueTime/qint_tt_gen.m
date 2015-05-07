% Generated code: DO NOT EDIT MANUALLY
% For use in generating Matlab/Simulink + TrueTime
% models from ESMoL models.  Please consult ESMoL
% documentation for additional information.


% Initialize TrueTime
init_truetime;
% Make the simulink library available
load_system('simulink');
% Make the truetime library available
load_system('truetime');
% Create a new simulink model for the system
new_system('qint_tt');
% Open the new model
open_system('qint_tt');
% Set the simulation time to 2000
set_param('qint_tt','StopTime', '40');



% Add a TT Network block for bus: 
add_block('truetime/TrueTime Network','qint_tt/TT_I2C');
set_param('qint_tt/TT_I2C','nwtype','TDMA');
set_param('qint_tt/TT_I2C','nwnbr','1');
set_param('qint_tt/TT_I2C','nnodes','2');
set_param('qint_tt/TT_I2C','rate','100000');
set_param('qint_tt/TT_I2C','minsize','512');
set_param('qint_tt/TT_I2C','lossprob','0');
set_param('qint_tt/TT_I2C','seed','0');
set_param('qint_tt/TT_I2C','schedule','[ 1 2 ]');
% Add a scope for scheduler output of the network block
add_block('simulink/Sinks/Scope','qint_tt/TT_I2C_Sched');
% Finally, connect the scheduler output to the scope
add_line('qint_tt','TT_I2C/1','TT_I2C_Sched/1','autorouting','off');




% Add a TT Kernel block for node RS
add_block('truetime/TrueTime Kernel','qint_tt/RS');
set_param('qint_tt/RS','sfun','RS_init');
set_param('qint_tt/RS','ninputsoutputs','[2 9]');
set_param('qint_tt/RS','nwnodenbr','[1 1]');
% Add a scope to the scheduler output of the kernel block
add_block('simulink/Sinks/Scope','qint_tt/RS_Sched');
% Finally, connect the scheduler output to the scope
add_line('qint_tt','RS/2','RS_Sched/1','autorouting','off');

% Add a TT Kernel block for node GS
add_block('truetime/TrueTime Kernel','qint_tt/GS');
set_param('qint_tt/GS','sfun','GS_init');
set_param('qint_tt/GS','ninputsoutputs','[0 0]');
set_param('qint_tt/GS','nwnodenbr','[1 2]');
% Add a scope to the scheduler output of the kernel block
add_block('simulink/Sinks/Scope','qint_tt/GS_Sched');
% Finally, connect the scheduler output to the scope
add_line('qint_tt','GS/1','GS_Sched/1','autorouting','off');




% Open the original source MDL file
%open_system('quad_integrator_subsys');
% Copy the plant from the source into the model
%add_block('quad_integrator_subsys/Plant', 'qint_tt/Plant');
%close_system('quad_integrator_subsys');
% Add wiring to/from the plant to the kernels



% Save and close the system
save_system('qint_tt');
close_system('qint_tt');