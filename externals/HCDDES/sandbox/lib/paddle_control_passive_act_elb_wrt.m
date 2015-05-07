% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = paddle_control_passive_act_elb_wrt(t,x,u,flag,id,T)

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
    paddle_calc_out(t, u, x, T);   
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
  sizes.NumOutputs     = 0;
  sizes.NumInputs      = 2;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  paddle_start(id,T)
  x0=[];
  str = [];
  ts = [T 0]; % Var sample time
end  

  
function paddle_start(id,T)

  global falcon
  if(isempty(falcon))
   falcon = haptikdevice;
  end 
end
     
%Haptik Force Here, or somewhere else
function sys = paddle_calc_out(t,u,x,T)

  global success
  global falcon
  
  a1 = u(1);
  a2 = u(2);
  a3 = 0;
  
  %Needs forces on it

  haptic_force(a1, a2, a3);
  
  %pos = read_position(falcon);

 % y(1) = pos(1,2) * (1/5);  %ELbow
  %y(2) = pos(1,3) * (1/5);  %Wrist

  %y(3) = read_button(falcon); %Button
  
  %Output?
  
end
       
function haptic_force(a1, a2, a3)
global falcon
if(~isempty(falcon))
write(falcon,[a3,a1,a2]);
end
%Do some Haptic Paddle Wizardry Here

end

function kill_paddle(t,u,x,T)
    global falcon
    
    disp('Paddle is now off');
if(~isempty(falcon))
    close(falcon)
    falcon=[];
end

end

