%
% Build tests for the Starmac (keep track of what we've tested).
%

samprate = 0.02;
maxtime = 120;
testnames = {};
tout = [0:samprate:maxtime]';

% Sine tests (originals) - stable
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = 2 * sin( 2 * .0625 * 0.25 * 2 * pi * tout );
yaw_ref = [tout yout];

i1_out = 2 * sin(.0625 * 2 * pi * tout );
i2_out = -2 * sin(.0625 * 2 * pi * tout );
i3_out = -1 * sin(.0625 * 2 * pi * tout );
inertial_ref = [tout i1_out i2_out i3_out];

save stable_sin_test tout yaw_ref inertial_ref
testnames = {testnames; 'stable_sin_test'};

% Sine tests (testing yaw bounds) - stable
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = 3 * sin( 2 * .0625 * 0.25 * 2 * pi * tout );
yaw_ref = [tout yout];

i1_out = 2 * sin(.0625 * 2 * pi * tout );
i2_out = -2 * sin(.0625 * 2 * pi * tout );
i3_out = -1 * sin(.0625 * 2 * pi * tout );
inertial_ref = [tout i1_out i2_out i3_out];

save stable_yaw_test tout yaw_ref inertial_ref
testnames = {testnames; 'stable_yaw_test'};

% Step test - x 
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = zeros(size(tout));
yaw_ref = [tout yout];

deadtime = [0:samprate:5];
i1_out = [zeros(size(deadtime)) ones(1,length(tout)-length(deadtime))]';
i2_out = zeros(size(tout));
i3_out = zeros(size(tout)); 
inertial_ref = [tout i1_out i2_out i3_out];

save step_x_test tout yaw_ref inertial_ref
testnames = {testnames; 'step_x_test'};

% Step test - y
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = zeros(size(tout));
yaw_ref = [tout yout];

deadtime = [0:samprate:5];
i1_out = zeros(size(tout));
i2_out = [zeros(size(deadtime)) ones(1,length(tout)-length(deadtime))]';
i3_out = zeros(size(tout)); 
inertial_ref = [tout i1_out i2_out i3_out];

save step_y_test tout yaw_ref inertial_ref
testnames = {testnames; 'step_y_test'};

% Ramp test - x 
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = zeros(size(tout));
yaw_ref = [tout yout];

i1_out = 3 * tout;
i2_out = zeros(size(tout));
i3_out = zeros(size(tout)); 
inertial_ref = [tout i1_out i2_out i3_out];

save ramp_x_test tout yaw_ref inertial_ref
testnames = {testnames; 'ramp_x_test'};

% Ramp test - y 
clear yout yaw_ref i1_out i2_out i3_out inertial_ref

yout = zeros(size(tout));
yaw_ref = [tout yout];

i1_out = zeros(size(tout));
i2_out = 3 * tout;
i3_out = zeros(size(tout)); 
inertial_ref = [tout i1_out i2_out i3_out];

save ramp_y_test tout yaw_ref inertial_ref
testnames = {testnames; 'ramp_y_test'};

save test_list testnames

clear
close all
