%%%%%%%  Aerodynamics Sim Run File  %%%%%%%%%%%%%%%
%%%%%%%  Haomiao Huang 6/13/2007    %%%%%%%%%%%%%%%

%%%%% Notes on state vector:
% [x y z xd yd zd p r y ax ay az omega_x omega_y omega_z]'
% Note: accelerations and angular rates are reported in body axes, and are 
% meant to represent raw IMU data, whereas the position, velocity, and
% Euler angles are meant to represent KF output.  The noise models for both
% differ accordingly.  

%%%%%% Notes on noise:
% Sensor noise:  zero-mean Gaussian noise is added to all of the state
% variables through the estimator block.  

% Motor noise: motor noise is currently implemented as zero-mean Gaussian
% noise with std. dev. of X% of nominal thrust, where X is set in the
% constant declaration

%%%%% Wind disturbance: 
% Wind can be modeled in one of two ways:
% Constant wind velocity vector with noise
% Wind gust: modeled as a 1 - cos gust which starts at 0 and builds up to 
% a maximum velocity before subsiding back to zero, also with noise.  The
% gust can be clipped using saturation to generate a wind that builds up 
% to a constant velocity for some period, then subsides.  For example,
% setting wind_max to 10 and wind_clip to 5 will generate a gust that
% builds up to 5 m/s, holds there for some time, then subsides.

clear;
clc
%%%%%%%%%% SIMULATION MODE SELECTION %%%%%%%%%%%%%%%
select = 0;         % attitude only control
% select = 1;         % position tracking control

t_final = 10;            % Simulation run time in seconds

%%%%%%%%%%%% Initial Conditions %%%%%%%%%%%%%%%%%%
X0 = [0 0 0]'; % initial position, inertial, m
Vb0 = [0 0 0]'; % initial velocity, body, m/s
T0 = [0 0 0]'; % initial attitude, euler roll pitch yaw, rad
W0 = [0 0 0]'; % initial angular rates, body, rad/s

%%%%%%%%%%%% Control Declarations %%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%% Z %%%%%%%%%%%%%%%%%%
z_ref = -.5;  % reference altitude
zd_ref = 0;  % reference climb rate
K_pz = .37; % z proportional gain
K_dz = 0.7;  % z derivative gain

%%%%%%%%%%% Attitude %%%%%%%%%%%%%%
kp_r = 1; % roll proportional gain
kd_r = 1; % roll derivative gain
ki_r = 0; % roll integral gain (not used)
kp_p = 1; % pitch proportional gain
kd_p = 1; % pitch derivative gain
ki_p = 0; % pitch integral gain (not used)
kp_y = 0; % yaw proportional gain
kd_y = 0; % yaw derivative gain
ki_y = 0; % yaw integral gain (not used)

% take gains from error signals to thrust outputs
K_attitude = [  0   kp_p   kp_y     0    kd_p   kd_y;
              -kp_r   0   -kp_y  -kd_r   0    -kd_y;
                0  -kp_p   kp_y    0    -kd_p  kd_y;
               kp_r   0   -kp_y   kd_r   0    -kd_y];
         
% reference attitude for attitude only control, [p r y pd rd yd]'
att_ref = [0.2 0.2 0 0 0 0]';

%%%%%%%%%%%%%% Wind controls %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% wind = 1;               % wind is turned off
% wind = 2;             % constant wind 
wind = 3;             % wind gust
const_wind = [1 1 0];   % wind velocity vector for constant wind, m/s
wind_start = 3;         % start time for wind, s
wind_duration = 5;      % duration of wind gust, s
wind_max = 10;          % amplitude of wind gust, m/s
wind_clip = wind_max;   % clip wind speed at this velocity to make gusts
wind_vector = [1 0 0]'; % direction of wind gust in NED
% wind_vector = rand(3,1); % direction of wind gust in NED, random
w_seed = 0;             % seed for random number gen for wind noise
wind_var = (wind_max / 10)^2; % wind noise variance, set to 10% of max wind

%%%%%%%%%%%% Run the Simulation %%%%%%%%%%%%%
constants;
sim('aero_sim', t_final);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% store individual variable names, note these are the "actual" states, as
% opposed to the noisy estimated states used by the controller
t = state.time;
x = state.signals.values(:,1);
y = state.signals.values(:,2);
z = state.signals.values(:,3);
vx = state.signals.values(:,4);
vy = state.signals.values(:,5);
vz = state.signals.values(:,6);
roll = state.signals.values(:,7);
pitch = state.signals.values(:,8);
yaw = state.signals.values(:,9);
ax = state.signals.values(:,10);
ay = state.signals.values(:,11);
az = state.signals.values(:,12);
wx = state.signals.values(:,13);
wy = state.signals.values(:,14);
wz = state.signals.values(:,15);

