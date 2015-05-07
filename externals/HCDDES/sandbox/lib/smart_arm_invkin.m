% Joseph Hall
% April 7, 2009
% S Function Inverse Kinematics for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = smart_arm_invkin(t,x,u,flag,id,T)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = ikin_init(id);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = [];
   case 3,
    %Output function
    sys = ikin_calc_out(t, u, x);    
   case 9,
    %Terminate function
    ikin_kill(t, u, x);
  end;

function [sys, x0, str, ts] = ikin_init(id)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0; % setup inputs and outputs
  sizes.NumOutputs     = 4;
  sizes.NumInputs      = 16;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  ikin_start(id);
  x0=[];
  str = [];
  ts = [-1 0]; % Var sample time

  
function ikin_start(id)

    disp('Smart Arm Inverse Kinematics has started');


function sys = ikin_calc_out(t,u,x)

d4=0.229;
d1=0.160;

% This area is for T1
px=u(13);
py=u(14);

t1= atan2(py, px); 
sys(1)=t1;

%This area is for T4
r21=u(2);
r11=u(1);
r22=u(6);
r12=u(5);

t4= atan2(r21*cos(t1) - r11*sin(t1), r22*cos(t1) - r12*sin(t1));

sys(4)=t4;
%This area is for T2

r33=u(11);
r23=u(10);
r13=u(9);
pz=u(15);

num2= d4*r33+d1-pz;
den2= (py-d4*r23)*sin(t1) +(px-d4*r13)*cos(t1);

t2= atan2(num2, den2);

sys(2)= t2;

%This area is for T3

num3= (r23*sin(t1) + r13*cos(t1))*cos(t2) - r33*sin(t2);
den3= (r23*sin(t1) + r13*cos(t1))*sin(t2) + r33*cos(t2);

t3= atan2(num3, den3);

sys(3)= t3;


function ikin_kill(t,u,x)


    disp('Smart Arm Inverse Kinematics is done');
