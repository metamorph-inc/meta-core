% Joseph Hall
% March 15, 2009
% S Function Controller for Crustcrawler Smart Arm Robot

function [sys, x0, str, ts] = etime_compare(t,x,u,flag,id,T)

  switch flag
   case 0,
    %Init function
    [sys, x0, str, ts] = init(id, T);
   case 1,
    %Derivative function
    sys = [];
   case 2,
    %Update function
    sys = [];
   case 3,
    %Output function
    sys = etime_output(t, u, x, T);   
   case 9,
    %Terminate function
    etime_terminate(t, u, x, T);
   otherwise
    error(['Unhandled flag = ', num2str(flag)]);
  end;

function [sys, x0, str, ts] = init(id, T)

  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0; % setup inputs and outputs
  sizes.NumOutputs     = 1;
  sizes.NumInputs      = 6;
  sizes.DirFeedthrough = 1; % Dir Feedthrough must be on
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  etime_init(id,T);
  x0=[];
  str = [];
  ts = [T 0]; % Var sample time

  
function etime_init(id, T)

disp('Etime compare is now on');
 
function sys = etime_output(t,u,x,T)


        for i=1:6
        
            c1(i) = u(i);         

        end
        
        c2=fix(clock);
        
        for i=1:1
           
            y(i)=etime(c2, c1);
            
        end
        
        sys=y;


function etime_terminate(t,u,x,T)

disp('Etime compare is now off');
  