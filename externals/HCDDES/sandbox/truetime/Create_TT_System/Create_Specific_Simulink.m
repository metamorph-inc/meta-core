function Create_Specific_Simulink(schedule, additional_rules, env)

% *********
% need to handle multiple networks
% additional information on how to link together the imported blocks with
% generated blocks.



% structure:
% 3 section, each 280 wide, first contains include non-generated
%  second contains nodes
%  third contains networks

sect1_offset = 0;
sect2_offset = 280;
sect3_offset = 560;

num_nodes = size(schedule.Nodes, 2);
num_networks = size(schedule.NetworkParameters, 2);
num_addnlrules = size(additional_rules,2);

load_system([env '\truetime']);

new_system('temp');

new_system(schedule.name);
open_system(schedule.name);

% first import all the non-generated stuff.
load_system('Additional_Import');
% this is a hack to copy from one block diagram to another
add_block('built-in/Subsystem', 'temp/subs');
Simulink.BlockDiagram.copyContentsToSubSystem('Additional_Import', 'temp/subs');
Simulink.SubSystem.copyContentsToBlockDiagram('temp/subs', schedule.name);
Simulink.BlockDiagram.deleteContents('temp');
close_system('Additional_Import');

% build the networks

for i=1:num_networks
    % for now we are going to work with the assumption that there is only
    % one network - otherwise would have to edit data structure
    k = int2str(i);
    % location of TT blocks
    add_block('built-in/Subsystem', [schedule.name '/TDMA Network' k]);
    set_param([schedule.name '/TDMA Network' k], 'Position', [(sect3_offset + 90) (30 + (i-1)*300) (sect3_offset + 100 + 90) (30 + (i-1)*300 + 20*num_nodes)]);
    set_param([schedule.name '/TDMA Network' k], 'BackgroundColor','cyan');

    % this builds the inner guts of the TDMA network block
    add_block('built-in/Subsystem', [schedule.name '/TDMA Network' k '/TrueTime Network']);
    % this is a hack to copy from one subsystem to another
    Simulink.SubSystem.copyContentsToBlockDiagram('truetime/TrueTime Network', 'temp');
    Simulink.BlockDiagram.copyContentsToSubSystem('temp', [schedule.name '/TDMA Network' k '/TrueTime Network']);
    Simulink.BlockDiagram.deleteContents('temp');

    % mask the created subsystem appropriately
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'Position', [170 20 285 96]);
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'Mask', 'on');
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'MaskPrompts', {'Network type','Network number','Number of nodes','Data rate (bits/s)','Minimum frame size (bits)','Loss probability (0-1)','Bandwidth allocations','Slotsize (bits)','Cyclic schedule','Total switch memory (bits)','Switch buffer type','Switch overflow behavior'});
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'MaskVariables', 'nwtype=@1;nwnbr=@2;nnodes=@3;rate=@4;minsize=@5;lossprob=@6;bandwidths=@7;slotsize=@8;schedule=@9;memsize=@10;buftype=@11;overflow=@12;');
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'MaskStyles', {'popup(CSMA/CD (Ethernet)|CSMA/AMP (CAN)|Round Robin|FDMA|TDMA|Switched Ethernet)','edit','edit','edit','edit','edit','edit','edit','edit','edit','popup(Common buffer|Symmetric output buffers)','popup(Retransmit|Drop)'});
    set_param([schedule.name '/TDMA Network' k '/TrueTime Network'], 'MaskValues', {schedule.NetworkParameters(i).network_type,...
                                                                                schedule.NetworkParameters(i).network_number,...
                                                                                schedule.NetworkParameters(i).num_nodes,...
                                                                                schedule.NetworkParameters(i).data_rate,...
                                                                                schedule.NetworkParameters(i).min_frame_size,...
                                                                                schedule.NetworkParameters(i).loss_prob,...
                                                                                schedule.NetworkParameters(i).bandwidth_alloc,...
                                                                                schedule.NetworkParameters(i).slotsize,...
                                                                                schedule.NetworkParameters(i).cyclic_sched,...
                                                                                schedule.NetworkParameters(i).total_switch_mem,...
                                                                                'Common Buffer','Retransmit'});

    add_block('built-in/Scope', [schedule.name '/TDMA Network' k '/Network_Schedule']);
    add_block('built-in/Mux', [schedule.name '/TDMA Network' k '/Mux1']);
    add_block('built-in/Demux', [schedule.name '/TDMA Network' k '/Demux1']);

    set_param([schedule.name '/TDMA Network' k '/Network_Schedule'], 'Position', [360 140 380 170]);
    set_param([schedule.name '/TDMA Network' k '/Mux1'], 'inputs', int2str(num_nodes));
    set_param([schedule.name '/TDMA Network' k '/Mux1'], 'Position', [70 40 80 80]);
    set_param([schedule.name '/TDMA Network' k '/Demux1'], 'outputs', int2str(num_nodes));
    set_param([schedule.name '/TDMA Network' k '/Demux1'], 'Position', [360 40 370 80]);

    add_line([schedule.name '/TDMA Network' k], 'Mux1/1', 'TrueTime Network/1');
    add_line([schedule.name '/TDMA Network' k], 'TrueTime Network/1', 'Demux1/1');
    add_line([schedule.name '/TDMA Network' k], 'TrueTime Network/2', 'Network_Schedule/1');
end

% add inputs and outputs to the network block for each node in the network
for i=1:num_nodes
    k = int2str(i);
    add_block('built-in/inport', [schedule.name '/TDMA Network1/inport' k]);
    add_block('built-in/outport', [schedule.name '/TDMA Network1/outport' k]);
    
    set_param([schedule.name '/TDMA Network1/inport' k], 'Position', [10 (10 + 30 * i) 40 (24 + 30 * i)]);
    set_param([schedule.name '/TDMA Network1/outport' k], 'Position', [400 (10 + 30 * i) 440 (24 + 30 * i)]);
    
    add_line([schedule.name '/TDMA Network1'], ['Demux1/' k], ['outport' k '/1']);
    add_line([schedule.name '/TDMA Network1'], ['inport' k '/1'], ['Mux1/' k]);
    
    % the goto and send around the network
    add_block('built-in/Goto', [schedule.name '/NGoto' k]);
    set_param([schedule.name '/NGoto' k], 'GotoTag', ['rcv' k]);
    set_param([schedule.name '/NGoto' k], 'Position', [(sect3_offset + 230) (10 + i*20) (sect3_offset + 230 + 30) (25 + i*20)]);
    add_block('built-in/From', [schedule.name '/NFrom' k]);
    set_param([schedule.name '/NFrom' k], 'GotoTag', ['snd' k]);
    set_param([schedule.name '/NFrom' k], 'Position', [(sect3_offset + 20) (10 + i*20) (sect3_offset + 20 + 30) (25 + i*20)]);

    add_line(schedule.name, ['TDMA Network1/' k],['NGoto' k '/1']);
    add_line(schedule.name, ['NFrom' k '/1'],['TDMA Network1/' k]);
end

for i=1:num_nodes
    k = int2str(i);
    % **** This will wrap the TT Kernel in the addnl inputs and outputs ****
    add_block('built-in/Subsystem', [schedule.name '/' schedule.Nodes(i).name]);
    set_param([schedule.name '/' schedule.Nodes(i).name], 'Position', [(sect2_offset + 90) (30 + (i - 1) * 60) (sect2_offset + 90 + 100) (72 + (i - 1) * 60)]);
    set_param([schedule.name '/' schedule.Nodes(i).name], 'BackgroundColor','green');
    
    % add the additional blocks
    add_block('built-in/Terminator', [schedule.name '/' schedule.Nodes(i).name '/Terminator1']);
    add_block('built-in/Terminator', [schedule.name '/' schedule.Nodes(i).name '/Terminator2']);
    add_block('built-in/Terminator', [schedule.name '/' schedule.Nodes(i).name '/Terminator3']);
    add_block('built-in/Ground', [schedule.name '/' schedule.Nodes(i).name '/Ground1']);
    add_block('built-in/Scope', [schedule.name '/' schedule.Nodes(i).name '/' schedule.Nodes(i).name '_Output']);
    add_block('built-in/Scope', [schedule.name '/' schedule.Nodes(i).name '/' schedule.Nodes(i).name '_Schedule']);
    add_block('built-in/inport', [schedule.name '/' schedule.Nodes(i).name '/input']);
    add_block('built-in/outport', [schedule.name '/' schedule.Nodes(i).name '/output']);
    % position the additional blocks
    set_param([schedule.name '/' schedule.Nodes(i).name '/Terminator1'], 'Position', [415 70 435 90]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/Terminator2'], 'Position', [420 215 440 235]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/Terminator3'], 'Position', [420 255 440 275]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/Ground1'], 'Position', [120 130 140 150]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/' schedule.Nodes(i).name '_Output'], 'Position', [415 14 445 46]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/' schedule.Nodes(i).name '_Schedule'], 'Position', [420 159 450 191]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/input'], 'Position', [55 108 85 122]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/output'], 'Position', [420 118 450 132]);

    % **** This will create the actual TT Kernel block ****
    
    % location of TT blocks
    add_block('built-in/Subsystem', [schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel']);
    % this is a hack to copy from one subsystem to another
    Simulink.SubSystem.copyContentsToBlockDiagram('truetime/TrueTime Kernel', 'temp');
    Simulink.BlockDiagram.copyContentsToSubSystem('temp', [schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel']);
    Simulink.BlockDiagram.deleteContents('temp');
    % mask the created subsystem appropriately (still need to add battery param)
    set_param([schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel'], 'Position', [200 102 315 178]);
    set_param([schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel'], 'Mask', 'on');
    set_param([schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel'], 'MaskPrompts', {'Name of init function (MEX or MATLAB)','Init function argument','Clock drift','Clock offset'});
    set_param([schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel'], 'MaskVariables', 'sfun=&1;args=@2;clockdrift=@3;clockoffset=@4;');
    set_param([schedule.name '/' schedule.Nodes(i).name '/TrueTime Kernel'], 'MaskValues', {'Generic_Init','schedule','0', '0'});

    % attach all the blocks together
    % attach the input either from the receive or from an analog input, depending...
    if(schedule.(schedule.Nodes(i).name).num_analog_inp > 0)
        add_line([schedule.name '/' schedule.Nodes(i).name], 'input/1','TrueTime Kernel/1');
        add_line([schedule.name '/' schedule.Nodes(i).name], 'Ground1/1','TrueTime Kernel/3');
    else
        add_line([schedule.name '/' schedule.Nodes(i).name], 'input/1','TrueTime Kernel/3');
        add_line([schedule.name '/' schedule.Nodes(i).name], 'Ground1/1','TrueTime Kernel/1');
        % add line between sends and gotos
        add_block('built-in/From', [schedule.name '/BFrom' k]);
        set_param([schedule.name '/BFrom' k], 'GotoTag', ['rcv' k]);
        set_param([schedule.name '/BFrom' k], 'Position', [(sect2_offset + 20) (40 + (i - 1) * 60) (sect2_offset + 20 + 30) (55 + (i - 1) * 60)]);
        add_line(schedule.name, ['BFrom' k '/1'],[schedule.Nodes(i).name '/1']);
    end
    
    add_line([schedule.name '/' schedule.Nodes(i).name], 'Ground1/1','TrueTime Kernel/2');
    
% **** need to fix this to allow both outputs and sending...
    if(schedule.(schedule.Nodes(i).name).num_analog_outp > 0)
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/1','output/1');
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/1',[schedule.Nodes(i).name '_Output/1']);
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/2','Terminator1/1');
    else
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/2','output/1');
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/2',[schedule.Nodes(i).name '_Output/1']);
        add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/1','Terminator1/1');
        % add line between sends and gotos
        add_block('built-in/Goto', [schedule.name '/BGoto' k]);
        set_param([schedule.name '/BGoto' k], 'GotoTag', ['snd' k]);
        set_param([schedule.name '/BGoto' k], 'Position', [(sect2_offset + 230) (40 + (i - 1) * 60) (sect2_offset + 230 + 30) (55 + (i - 1) * 60)]);
        add_line(schedule.name, [schedule.Nodes(i).name '/1'],['BGoto' k '/1']);
    end
    
    add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/3',[schedule.Nodes(i).name '_Schedule/1']);
    add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/4','Terminator2/1');
    add_line([schedule.name '/' schedule.Nodes(i).name], 'TrueTime Kernel/5','Terminator3/1');
end

% handle all additional attachement rules that are included:
for i=1:num_addnlrules
    add_line([schedule.name], [additional_rules(i).from '/1'], [additional_rules(i).to '/1']);
end

close_system('truetime',0);
close_system('temp',0);
% save_system();
