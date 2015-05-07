function srobot = crust_crawler_robot()
% Joseph Hall
% March 25, 2009

% This file sets up a new robot called Smartarm. It uses the conventions 
% described by Peter I. Corke

clear Params

% Units are in Meters

% This is incorrect (its in inches)
%Params{1} = link([0         0      0      .630], 'mod');
%Params{2} = link([-pi/2     0      0       0], 'mod');
%Params{3} = link([0        .675    0      0], 'mod');
%Params{4} = link([pi/2      0      0      0.9], 'mod');

% This is incorrect (its in inches)
a=0;al=-pi/2;d=2.54e-2*4.5;th=0;
Params{1} = link([al,a,th,d], 'standard');
a=2.54e-2*6.75;al=0;d=0;th=0;
Params{2} = link([al,a,th,d], 'standard');
a=-2.54e-2*1;al=pi/2;d=0;th=pi/2; % XXX
Params{3} = link([al,a,th,d], 'standard');
a=0;al=0;d=2.54e-2*8.25;th=0;
Params{4} =  link([al,a,th,d], 'standard');

% Moment of Inertia parameters

Params{1}.I = [0   0  0   0 0 0];
Params{2}.I = [0   0  0   0 0 0];
Params{3}.I = [0   0  0   0 0 0];
Params{4}.I = [0   0  0   0 0 0]; % I am going to make these zero


%Mass Parameters in Kilograms (I'm guessing these right now)
%Params{1}.m = 0;
%Params{2}.m = 5;
%Params{3}.m = 3;
%Params{4}.m = 1;
%Params{5}.m = 0.5;
%Params{6}.m = 0;

Params{1}.m = .3624;  %0-1 362.4g
Params{2}.m = .4012;  %1-2 m1 
Params{3}.m = .2364*0.25;  %2-3 m2 401.2g
Params{4}.m = .2364*0.75;  %3-4 m3 236.4g

%Centers of Gravity (Mass) --- See notes 
Params{1}.r = [0, 0, (4.5*2.54e-2)/2];
Params{2}.r = [(6.75*2.54e-2)/2,   0,   0];
Params{3}.r = [(2.75*2.54e-2)/2, 0,  0];   
Params{4}.r = [0, 0, (8.25*2.54e-2)/2];

%Gear Ratio Parameters
Params{1}.G =  1;
Params{2}.G =  1;
Params{3}.G =  1;
Params{4}.G =  1;    % this robot has no gear ratios (that I know of)

%Motor Inertia Paramters
Params{1}.Jm =  8e-3;
Params{2}.Jm =  8e-3;
Params{3}.Jm =  8e-3;   % We will assume this motor inertia for now
Params{4}.Jm =  8e-3;

% V Friction 
Params{1}.B =   100e-3;
Params{2}.B =   100e-3;
Params{3}.B =   100e-3;
Params{4}.B =   100e-3;


%Set up the Robot
srobot = robot(Params, 'SmartArm', 'CrustCrawlercom', ' ');
srobot.name = 'SmartArm';