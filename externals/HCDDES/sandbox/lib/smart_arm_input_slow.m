% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = smart_arm_input_slow(t,x,u,flag,id,T)

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
    smart_arm_calc_out(t, u, x, T);   
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
  sizes.NumOutputs     = 0;
  sizes.NumInputs      = 4;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  smart_arm_start(id,T)
  x0=[];
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
     

function sys = smart_arm_calc_out(t,u,x,T)

  global success
  limit = 511; % should be 511 for all
  limit2 = 511; % 300
  limit3 = 511;  
  limit4 = 511; 

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
        
        % for joint 2
        if( sign(u(2)) == 1)
            calllib('dynamixel','dxl_write_word', 2,30, 512-limit2); % current position is max (pos direction)
            calllib('dynamixel','dxl_write_word', 2,34, abs(int32( u(2) )) ); % high limit torque
            
            calllib('dynamixel','dxl_write_word', 3,30, 512+limit2); % Opposite servo goes opposite direction
            calllib('dynamixel','dxl_write_word', 3,34, abs(int32( u(2) )) ); % high torque limit for opposing servo
        else
            calllib('dynamixel','dxl_write_word', 2,30, 512+limit2); % current position is max (neg direction)
            calllib('dynamixel','dxl_write_word', 2,34, abs(int32( u(2) )) ); % high limit torque
            
            calllib('dynamixel','dxl_write_word', 3,30, 512-limit2); % Opposite servo goes opposite direction
            calllib('dynamixel','dxl_write_word', 3,34, abs(int32( u(2) )) ); % high torque limit for opposing servo
        end        

        % for joint 3        
        if( sign(u(3)) == 1)
            calllib('dynamixel','dxl_write_word', 4,30, 512+limit3); % current position is max (pos direction)
            calllib('dynamixel','dxl_write_word', 4,34, abs(int32( u(3) )) ); % high limit torque
            
            calllib('dynamixel','dxl_write_word', 5,30, 512-limit3); % Opposite servo goes opposite direction
            calllib('dynamixel','dxl_write_word', 5,34, abs(int32( u(3) )) ); % high torque limit for opposing servo
        else
            calllib('dynamixel','dxl_write_word', 4,30, 512-limit3); % current position is max (neg direction)
            calllib('dynamixel','dxl_write_word', 4,34, abs(int32( u(3) )) ); % high limit torque
            
            calllib('dynamixel','dxl_write_word', 5,30, 512+limit3); % Opposite servo goes opposite direction
            calllib('dynamixel','dxl_write_word', 5,34, abs(int32( u(3) )) ); % high torque limit for opposing servo
        end         
        
        if( sign(u(4)) == 1)
            calllib('dynamixel','dxl_write_word', 6,30, 512+limit4); % current position is max (pos direction)
            calllib('dynamixel','dxl_write_word', 6,34, abs(int32(u(4))) ); % high limit torque
        else
            calllib('dynamixel','dxl_write_word', 6,30, 512-limit4); % current position is max (neg direction)
            calllib('dynamixel','dxl_write_word', 6,34, abs(int32(u(4))) ); % high limit torque
        end             
        
   end
       
function smart_arm_kill_robot(t,u,x,T)


    disp('Smart Arm is now off');
    
    calllib('dynamixel','dxl_write_word', 2,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 2,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 3,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 3,30,512); % reset robot   
 
    pause(1);
    
    calllib('dynamixel','dxl_write_word', 4,34, 1023); % high limit torque
    calllib('dynamixel','dxl_write_word', 4,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 5,34, 1023); % high limit torque
    calllib('dynamixel','dxl_write_word', 5,30,512); % reset robot   
    
    pause(1);
    
    calllib('dynamixel','dxl_write_word', 1,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 1,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 6,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 6,30,512); % reset robot  
    
    
    calllib('dynamixel','dxl_terminate'); % call libraries to turn off robot
    unloadlibrary('dynamixel');
