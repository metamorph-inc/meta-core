% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = smart_arm_output_HCI_elb_wrt(t,x,u,flag,id,T)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = smart_arm_init(id, T);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = [];
   case 3,
    %Output function
    sys = smart_arm_calc_out(t, u, x, T);   
   case 9,
    %Terminate function
    smart_arm_kill_robot(t, u, x, T);
   otherwise
    error(['Unhandled flag = ', num2str(flag)]);
  end;

function [sys, x0, str, ts] = smart_arm_init(id, T)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0; % setup inputs and outputs
  sizes.NumOutputs     = 4;
  sizes.NumInputs      = 0;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  smart_arm_start(id,T)
  x0=[];
  str = [];
  ts = [T 0]; % Var sample time

  
function smart_arm_start(id,T)


function sys = smart_arm_calc_out(t,u,x,T)

  global success
  
   if success == 1
           
            y(1) = double(calllib('dynamixel','dxl_read_word',1,36)); %actual position 
            y(2) = double(calllib('dynamixel','dxl_read_word',3,36)); %actual position 
            y(3) = double(calllib('dynamixel','dxl_read_word',4,36)); %actual position 
            y(4) = double(calllib('dynamixel','dxl_read_word',6,36)); %actual position
            
            y(1)=y(1)-512;
            %y(2)=y(2)- 400;
            y(2)=y(2)+ 380;
            y(3)=y(3)-170; 
            y(4)=y(4)-512;            
            
%             if (u(1) < 0)
%                 y(1+7)=-y(1+7);               
%             end
%             if (u(2) < 0)
%                 y(3+7)=-y(3+7);               
%             end
%             
%             if (u(4) < 0)
%                 y(6+7)=-y(6+7); 
%             end
            
        sys = y;
        
   end

       
function smart_arm_kill_robot(t,u,x,T)

