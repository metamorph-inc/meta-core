% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = smart_arm_one_joint(t,x,u,flag,id,T)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = smart_arm_init(id, T);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = smart_arm_calc_out(t, u, x, T);    
   case 3,
    %Output function
    sys = x;
   case 9,
    %Terminate function
    smart_arm_kill_robot(t, u, x, T);
   otherwise
    error(['Unhandled flag = ', num2str(flag)]);
  end;

function [sys, x0, str, ts] = smart_arm_init(id, T)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 3; % setup inputs and outputs
  sizes.NumOutputs     = 3;
  sizes.NumInputs      = 1;
  sizes.DirFeedthrough = 0; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  smart_arm_start(id,T);
  x0=[0 0 0];
  str = [];
  ts = [T 0]; % Var sample time

  
function smart_arm_start(id,T)

  global success
  
  loadlibrary('dynamixel','dynamixel.h');
  libfunctions('dynamixel');                 % initialize the robot
  success = calllib('dynamixel','dxl_initialize');
  
  if success == 1
      disp('Smart Arm is initialized');
  else
      disp('Smart Arm failed to initialize');
      return;
  end
  pause(1);

function sys = smart_arm_calc_out(t,u,x,T)

  global success
  limit = 511; % should be 511 for all

   if success == 1

        calllib('dynamixel','dxl_write_word',254,32,0); % all velocities max
        
        % for joint 1
        if( sign(u(1)) == 1)
            calllib('dynamixel','dxl_write_word', 1,30, 512+limit); % current position is max (pos direction)
            calllib('dynamixel','dxl_write_word', 1,34, abs(int32(u(1))) ); % high limit torque
        else
            calllib('dynamixel','dxl_write_word', 1,30, 512-limit); % current position is max (neg direction)
            calllib('dynamixel','dxl_write_word', 1,34, abs(int32(u(1))) ); % high limit torque
        end
     
        
        % Get all outputs
        
            y(1) = double(calllib('dynamixel','dxl_read_word',1,36)); %actual position            
            y(2) = double(calllib('dynamixel','dxl_read_word',1,38)); %actual velocity
            y(3) = double(calllib('dynamixel','dxl_read_word',1,40)); %load (can be converted to torque)              
            
        
            y(1)=y(1)+ 20 -512;       
            
            if (u(1) < 0)
                y(2)=-y(2);               
            end
            
        sys = y;
        
   end

       
function smart_arm_kill_robot(t,u,x,T)


    disp('Smart Arm is now off');
    
    for i=1:6
        calllib('dynamixel','dxl_write_word', i,30,512); % reset robot
    end
    calllib('dynamixel','dxl_terminate'); % call libraries to turn off robot
    unloadlibrary('dynamixel');
