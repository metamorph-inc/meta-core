% Constant declarations for aerodynamic simulation.
%
% Note: all coordinates assume NED as inertial frame and x-forward,
% y-right, z-down as body frame

%%%%%%%%%%%% General Constants %%%%%%%%%%% 
tau_motor = 10;        % motor time constant, 1/s
thrust_max  = 9.8 * 1; % motor saturation max, N
thrust_min  = 0;       % motor saturation min, N
K_torque = 0.47;          % motor thrust to torque
K_a1s = .009;          % coeff for linear flap model, rad / (m/s)
K_beta = 0.23;         % blade stiffness coefficient, Nm/rad
b = 0.1;              % viscous drag coefficient, estimated, N s/m
g = [0 0 9.8]';        % gravitational acceleration vector
v_min = 1e-3;          % if speed is less than v_min, pass through zero
R_90 = [0 -1 0; 
        1 0 0; 
        0 0 1];        % rotation matrix for 90 deg rotation
    
rho = 1.2; % kg/m^3, density of air
R = 0.127; % m, radius of each propellor
A = pi*R^2; % m^2, area swept out by 1 rotor

%%%%%%%%%%% Advance Ratio Induced Velocity Calculation %%%%%

% Analytic solution for v_i
% syms vh vi vlon vver 
% vi = solve('vi=vh^2/sqrt((vlon)^2+(vver+vi)^2)','vi');
% Define ranges for v_i lookup table
T = 9.8*[0.3:0.02:0.8]; % N
Vh = sqrt(T/(2*rho*A)); % m/s
Vinf = [0:0.5:6]; % m/s
Alpha = pi/180*[-85:5:85];

Vlon = Vinf'*cos(Alpha);
Vver = Vinf'*sin(Alpha);

for i = 1:length(Vh)
    for j = 1:length(Vinf)
        for k = 1:length(Alpha)
            Vi(k,j,i) = findVI(Vh(i),Vinf(j),Alpha(k));
            ToverTh(k,j,i) = min(10,max(0.1,Vh(i)/(Vi(k,j,i)+Vinf(j)*sin(Alpha(k)))));
        end
    end
end

% Plot for a given Vh
% figure(1);clf;
% for i=1:length(Vh)
% %     contourf(Vinf'*ones(size(Alpha)), ones(size(Vinf))'*Alpha, squeeze(Vi(:,:,i))');
%     contourf(Vinf'*ones(size(Alpha)), ones(size(Vinf))'*Alpha, squeeze(ToverTh(:,:,i))',logspace(-1,1));
%     str = strcat('Vh = ', num2str(Vh(i)));
%     title(str);
%     xlabel('Vinf')
%     ylabel('Alpha')
%     F(i) = getframe;
% end
% figure(2);
% movie(F,1,2)
% movie2avi(F,'ToverTh','compression','none','fps',3);
%%%%%%%%%%% Noise, Sensor, & Electronics Constnats %%%%%%%%%
t_s = 1/76;% controller & estimator sampling rate, s
pos_noise = noise_on*0.02^2;     % position estimation noise variance, m^2
vel_noise = noise_on*0.05^2;     % velocity est. noise variance, m^2
att_noise = noise_on*(.25*pi/180)^2; % attitude noise variance, rad^2
accel_noise = noise_on*.01^2;    % IMU accelerometer noise var, m/s/s ^2
gyro_noise = noise_on*.01^2;     % IMU angular velocity noise var, rad/s ^2
motor_noise = noise_on*(9.8/4*.05)^2; % Motor noise variance, ~5% of nominal

%%%%%%%%%%% Fake plant constant %%%%%%%%%%%%%%%%%%%%%%

plant_int_period = t_s / tau_motor;  % discretized plant sample period


%%%%%%%%%% PC104 Constants %%%%%%%%%%%%%%%
dx = .3048;             % distance of motor away from c.g. horizontally
% dz = .01325;
dz = .07;            % height of motor above c.g.
r1 = [dx 0 -dz]';       % moment arm to motor 1, m
r2 = [0 dx -dz]';
r3 = [-dx 0 -dz]';
r4 = [0 -dx -dz]';
M = 1.8263;             % mass, kg, with PC104
Jxx = 0.02759;          % Moment of inertia, x axis, kg m^2
Jyy = 0.02771;
Jzz = 0.04806;
J = [Jxx 0 0;
    0 Jyy 0;
    0 0 Jzz];           % inertia matrix in body frame, kg m^2
T_nom  = M*9.8/4;       % nominal thrust, N

K_flap = 4*(K_beta + T_nom*dz)*K_a1s/(2*dx);

%%%%%%%%%% Stargate Only Constants %%%%%%%%%%%
% dx = .3048; % distance of motor away from c.g. horizontally
% dz = .0248; % height of motor above c.g.
% r1 = [rx 0 -rz]'; % moment arm to motor 1, m
% r2 = [0 rx -rz]';
% r3 = [-rx 0 -rz]';
% r4 = [0 -rx -rz]';
% M = 1.1574; % mass, kg, without PC104
% J = [0.02683 0 0;
%     0 0.02667 0;
%     0 0 0.0471];
% T_nom  = M*9.8/4; % nominal thrust, N
