%%  Aerodynamics Sim Init File  %%%%%%%%%%%%%%%
%%%  Haomiao Huang 6/13/2007    %%%%%%%%%%%%%%%
%%%  Formatted by Steve 6/25/2007    %%%%%%%%%%%%%%%

%%% Notes on state vector:
% [x y z xd yd zd p r y ax ay az omega_x omega_y omega_z]'
% Note: accelerations and angular rates are reported in body axes, and are 
% meant to represent raw IMU data, whereas the position, velocity, and
% Euler angles are meant to represent KF output.  The noise models for both
% differ accordingly.  

%%% Notes on noise:
% Sensor noise:  zero-mean Gaussian noise is added to all of the state
% variables through the estimator block.  

% Motor noise: motor noise is currently implemented as zero-mean Gaussian
% noise with std. dev. of X% of nominal thrust, where X is set in the
% constant declaration

%%% Wind disturbance: 
% Wind can be modeled in one of two ways:
% Constant wind velocity vector with noise
% Wind gust: modeled as a 1 - cos gust which starts at 0 and builds up to 
% a maximum velocity before subsiding back to zero, also with noise.  The
% gust can be clipped using saturation to generate a wind that builds up 
% to a constant velocity for some period, then subsides.  For example,
% setting wind_max to 10 and wind_clip to 5 will generate a gust that
% builds up to 5 m/s, holds there for some time, then subsides.

clear;

%% SIMULATION SCENARIO SELECTION %%%%%%%%%%%%%%%

%%%% Select scenario, per predefined options in set_scenario.m

select = 3; % 1-No motion, 2-attitude step, 3-att/alt step, >3-tbd  
noise_on = 1; % Enable noise or not

%%% Default Initial Conditions %%%%%%%%%%%%%%%%%%
X0 = [0.1 0.2 0]'; % initial position, inertial, m
Vb0 = [0 0 0]'; % initial velocity, body, m/s
T0 = [0 0 0]'; % initial attitude, euler roll pitch yaw, rad
W0 = [0 0 0]'; % initial angular rates, body, rad/s

%%% Default reference attitude for attitude only control, [p r y pd rd yd]'
att_ref = [0.0 0.0 0 0 0 0]';

%%% Set the scenario parameters per the above selection
[att_traj_on, att_traj, alt_traj] = set_scenario(select);

%% Control Declarations %%%%%%%%%%%%%%%%%%%%
%%% Altitude %%%%%%%%%%%%%%%%%%
z_ref = -0.0;  % reference altitude
zd_ref = 0;  % reference climb rate
K_pz = .37; % z proportional gain
K_dz = 0.7;  % z derivative gain

%%% Attitude %%%%%%%%%%%%%%
kp_r = 1; % roll proportional gain
kd_r = 1; % roll derivative gain
ki_r = 0; % roll integral gain (not used)
kp_p = 1; % pitch proportional gain
kd_p = 1; % pitch derivative gain
ki_p = 0; % pitch integral gain (not used)
kp_y = 2; % yaw proportional gain
kd_y = 2; % yaw derivative gain
ki_y = 0; % yaw integral gain (not used)

% take gains from error signals to thrust outputs
K_attitude = [  0   kp_p  -kp_y     0    kd_p -kd_y;
              -kp_r   0    kp_y  -kd_r   0     kd_y;
                0  -kp_p  -kp_y    0    -kd_p -kd_y;
               kp_r   0    kp_y   kd_r   0     kd_y];

K_pos = 0.05*[0 -1 0 -2; 1 0 2 0];           
           
%% Wind controls %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% wind = 1;               % wind is turned off
% wind = 2;             % constant wind 
wind = 1;             % wind gust
const_wind = [1 1 0];   % wind velocity vector for constant wind, m/s
wind_start = 3;         % start time for wind, s
wind_duration = 5;      % duration of wind gust, s
wind_max = 10;          % amplitude of wind gust, m/s
wind_clip = wind_max;   % clip wind speed at this velocity to make gusts
wind_vector = [1 0 0]'; % direction of wind gust in NED
% wind_vector = rand(3,1); % direction of wind gust in NED, random
w_seed = 0;             % seed for random number gen for wind noise
wind_var = (wind_max / 10)^2; % wind noise variance, set to 10% of max wind

%% Define vehicle constants
constants;
