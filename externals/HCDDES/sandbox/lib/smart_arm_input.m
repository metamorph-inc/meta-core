% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = smart_arm(t,x,u,flag,id,T)

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
  limit1 = 511; % should be 511 for all
  limit2 = 511; % 
  limit3 = 511;  
  limit4 = 511; 
  num_act=int32(6);
  temp_byte=uint8(0);

   if success == 1

        calllib('dynamixel','dxl_write_word',254,32,0); % all velocities max
        
        stor(1)=abs(int32(u(1)));  
        
        stor(2)=abs(int32(u(2)));
        stor(3)=abs(int32(u(2)));
        
        stor(4)=abs(int32(u(3))); %store torques to send in sync packet
        stor(5)=abs(int32(u(3)));
        
        stor(6)=abs(int32(u(4)));
        
        % for joint 1
        if( sign(u(1)) == 1) %Store position for J1 in sync packet

            spos(1)=512+limit1;
                
        else

            spos(1)=512-limit1;
     
        end
        
        % for joint 2
        if( sign(u(2)) == 1) %Store position for J2 in sync packet
            
              spos(2)=512-limit2;
              spos(3)=512+limit2;  

        else

              spos(2)=512+limit2;
              spos(3)=512-limit2;

        end        

        % for joint 3        
        if( sign(u(3)) == 1) %Store position for J3 in sync packet

              spos(4)=512+limit3;
              spos(5)=512-limit3;

        else

              spos(4)=512-limit3;
              spos(5)=512+limit3;

        end         
        
        if( sign(u(4)) == 1) %Store position for J4 in sync packet

              spos(6)=512+limit4;

        else

              spos(6)=512-limit4;
        end             
        
      
        calllib('dynamixel','dxl_set_txpacket_id', 254); % Broadcast ID
        calllib('dynamixel','dxl_set_txpacket_instruction', 131); %Synchwrite          
        calllib('dynamixel','dxl_set_txpacket_parameter', 0, 30); % Write to Position 5=parameter
        calllib('dynamixel','dxl_set_txpacket_parameter', 1, 6); % Write six bytes
        calllib('dynamixel','dxl_set_txpacket_length', int32((6+1)*num_act+4)); % length            
      
        
        for i=0:(num_act-1)
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i), i+1); % id of motor
           %send position
           
           temp_byte=uint8(calllib('dynamixel','dxl_get_lowbyte',spos(i+1)));
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+1, temp_byte);
           
           temp_byte=uint8(calllib('dynamixel','dxl_get_highbyte',spos(i+1)));           
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+2, temp_byte);
 
           temp_byte=uint8(calllib('dynamixel','dxl_get_lowbyte', 0));            
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+3, temp_byte);
 
           temp_byte=uint8(calllib('dynamixel','dxl_get_highbyte', 0));            
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+4, temp_byte); 
           
           temp_byte=uint8(calllib('dynamixel','dxl_get_lowbyte',stor(i+1)));            
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+5, temp_byte);
           
           temp_byte=uint8(calllib('dynamixel','dxl_get_highbyte',stor(i+1)));            
           calllib('dynamixel','dxl_set_txpacket_parameter', 2+(7*i)+6, temp_byte);            
           
        end
               
        calllib('dynamixel','dxl_txrx_packet'); %Send the packet    
        
   end
       
function smart_arm_kill_robot(t,u,x,T)


    disp('Smart Arm is now off');
    
    calllib('dynamixel','dxl_write_word', 2,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 2,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 3,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 3,30,512); % reset robot   
 
    pause(2);
    
    calllib('dynamixel','dxl_write_word', 4,34, 1023); % high limit torque
    calllib('dynamixel','dxl_write_word', 4,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 5,34, 1023); % high limit torque
    calllib('dynamixel','dxl_write_word', 5,30,512); % reset robot   
    
    pause(2);
    
    calllib('dynamixel','dxl_write_word', 1,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 1,30,512); % reset robot    
    calllib('dynamixel','dxl_write_word', 6,34, 700); % high limit torque
    calllib('dynamixel','dxl_write_word', 6,30,512); % reset robot  
    
    
    calllib('dynamixel','dxl_terminate'); % call libraries to turn off robot
    unloadlibrary('dynamixel');
