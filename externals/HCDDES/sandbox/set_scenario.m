function [att_traj_on, att_traj, alt_traj] = set_scenario(select)

switch (select) 
    case 1 % Open loop hover
        att_traj_on = 1;
        %%% Attitude Trajectory
        att_traj.time = [0:1:20]';
        att_traj.signals.values = zeros(21,6); % Set all attitude and attitude rate commands to zero
        att_traj.signals.dimension = [21 6];

        %%% Altitude Trajectory
        alt_traj.time = [0:1:20]';
        alt_traj.signals.values = zeros(21,1);
        alt_traj.signals.dimension = [21 1];
    case 2 % Blade flapping pitch demo
        att_traj_on = 1;
        %%% Attitude Trajectory
        att_traj.time = [0:1:12]';
        att_traj.signals.values = zeros(13,6); % Set all attitude and attitude rate commands to zero
        att_traj.signals.values(1:4,2) = 7*ones(4,1); % Roll
        att_traj.signals.values(5:12,2) = 29*ones(8,1); % Roll
        att_traj.signals.values(1:11,1) = 0.0*ones(11,1); % Pitch
        att_traj.signals.values(1:11,3) = 0.0*ones(11,1); % Yaw
        att_traj.signals.values = pi/180*att_traj.signals.values; % Convert to radians
        att_traj.signals.dimension = [13 6];

        %%% Altitude Trajectory
        alt_traj.time = [0:1:12]';
        alt_traj.signals.values = zeros(13,1);
        alt_traj.signals.values(1:13,1) = -0.0*ones(13,1); % Altitude
        alt_traj.signals.dimension = [13 1];

    case 3 % Translational Popup Flight demo
        att_traj_on = 1;
        %%% Attitude Trajectory
        att_traj.time = [0:0.5:6]';
        att_traj.signals.values = zeros(13,6); % Set all attitude and attitude rate commands to zero
        att_traj.signals.values(1:5,1) = -19*ones(5,1); % Roll
        att_traj.signals.values(6:13,1) = 19*ones(8,1); % Roll
        att_traj.signals.values(1:11,2) = 0.0*ones(11,1); % Pitch
        att_traj.signals.values(1:11,3) = 0.0*ones(11,1); % Yaw
        att_traj.signals.values = pi/180*att_traj.signals.values; % Convert to radians
        att_traj.signals.dimension = [13 6];

        %%% Altitude Trajectory
        alt_traj.time = [0:1:6]';
        alt_traj.signals.values = zeros(7,1);
        alt_traj.signals.values(1:7,1) = -0.0*ones(7,1); % Altitude
        alt_traj.signals.dimension = [7 1];

    case 4 % Pitch Sine wave input
        att_traj_on = 1;
        %%% Attitude Trajectory
        att_traj.time = [0:0.1:20]';
        att_traj.signals.values = zeros(201,6); % Set all attitude and attitude rate commands to zero
        freq = 0.3; % Hz
        att_traj.signals.values(:,1) = 15*sin(freq*att_traj.time*2*pi); % Roll
        att_traj.signals.values = pi/180*att_traj.signals.values; % Convert to radians
        att_traj.signals.dimension = [201 6];

        %%% Altitude Trajectory
        alt_traj.time = [0:1:20]';
        alt_traj.signals.values = zeros(21,1);
        alt_traj.signals.dimension = [21 1];

    case 5 % Hover
        att_traj_on = 0;
        att_traj.time = [0:0.1:20]';
        att_traj.signals.values = zeros(201,6); % Set all attitude and attitude rate commands to zero
        alt_traj.time = [0:1:20]';
        alt_traj.signals.values = zeros(21,1);
        
end
