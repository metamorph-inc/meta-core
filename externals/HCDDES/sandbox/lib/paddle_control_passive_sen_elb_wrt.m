% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = paddle_control_passive_sen_elb_wrt(t,x,u,flag,id,T)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = paddle_init(id, T);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = [];
   case 3,
    %Output function
   sys = paddle_calc_out(t, u, x, T);   
   case 9,
    %Terminate function
    kill_paddle(t, u, x, T);
   otherwise
    error(['Unhandled flag = ', num2str(flag)]);
  end;
end  

function [sys, x0, str, ts] = paddle_init(id, T)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0; % setup inputs and outputs
  sizes.NumOutputs     = 2;
  sizes.NumInputs      = 0;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  paddle_start(id,T)
  x0=[];
  str = [];
  ts = [T 0]; % Var sample time
end  

  
function paddle_start(id,T)

  global success
  global falcon
  
  if(isempty(falcon))
  %Attempt to load haptiklib
  falcon = haptikdevice;
  end
end
     
%Haptik Force Here, or somewhere else
function sys = paddle_calc_out(t,u,x,T)

  global success
  global falcon
  
  %a1 = u(1);
  %a2 = 0;
  %a3 = 0;
  
  %Needs forces on it
 % haptic_force(a1, a2, a3);
  if(~isempty(falcon))
  pos = 1.0e-3*read_position(falcon);

  y(1) = pos(1,2);  %ELbow
  y(2) = pos(1,3)-0.015;  %Wrist

  %y(3) = read_button(falcon); %Button
  
  %Output?
  sys = y;
  else
  sys=0;
  end
end
       

function kill_paddle(t,u,x,T)
    global falcon
    if(~isempty(falcon))
    disp('Paddle is now off');
    close(falcon)
    falcon=[];
    end
end    
