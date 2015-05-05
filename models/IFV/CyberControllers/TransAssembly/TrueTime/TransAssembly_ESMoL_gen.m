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
new_system('TransAssembly_ESMoL');
% Open the new model
open_system('TransAssembly_ESMoL');
% Set the simulation time to 2000
set_param('TransAssembly_ESMoL','StopTime', '40');






% Add a TT Kernel block for node TCU
add_block('truetime/TrueTime Kernel','TransAssembly_ESMoL/TCU');
set_param('TransAssembly_ESMoL/TCU','args','[1]');
set_param('TransAssembly_ESMoL/TCU','sfun','TCU_init');
set_param('TransAssembly_ESMoL/TCU','ninputsoutputs','[1 1]');
set_param('TransAssembly_ESMoL/TCU','nwnodenbr','[]');
% Add a scope to the scheduler output of the kernel block
add_block('simulink/Sinks/Scope','TransAssembly_ESMoL/TCU_Sched');
% Finally, connect the scheduler output to the scope
add_line('TransAssembly_ESMoL','TCU/2','TCU_Sched/1','autorouting','off');




% Open the original source MDL file
open_system('quad_integrator_subsys');
% Copy the plant from the source into the model
add_block('quad_integrator_subsys/Plant', 'TransAssembly_ESMoL/Plant');
close_system('quad_integrator_subsys');
% Add wiring to/from the plant to the kernels



% Save and close the system
save_system('TransAssembly_ESMoL');
close_system('TransAssembly_ESMoL');