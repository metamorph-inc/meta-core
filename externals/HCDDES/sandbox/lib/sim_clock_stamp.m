% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = sim_clock_stamp(t,x,u,flag,id,T,n)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = init(id, T, n);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = [];
   case 3,
    %Output function
    sys = stamp_output(t, u, x, T, n);   
   case 9,
    %Terminate function
    stamp_terminate(t, u, x, T);
   otherwise
    error(['Unhandled flag = ', num2str(flag)]);
  end;

function [sys, x0, str, ts] = init(id, T, n)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0; % setup inputs and outputs
  sizes.NumOutputs     = n+6;
  sizes.NumInputs      = n;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  stamp_init(id,T);
  x0=[];
  str = [];
  ts = [T 0]; % Var sample time

  
function stamp_init(id, T)

disp('Clock stamp is now on');
 
function sys = stamp_output(t,u,x,T,n)


        for i=1:n
        
            y(i) = u(i);         

        end
        
        c=fix(clock);
        
        for i=1:6
           
            y(i+n)=c(i);
            
        end
        
        sys=y;


function stamp_terminate(t,u,x,T)

disp('Clock stamp is now off');
  