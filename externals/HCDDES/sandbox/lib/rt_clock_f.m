%%%%%%%%%%%%%%%%%%%%%%%%%%
% S-Function Main
%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys, x0, str, ts] = rt_clock_f(t,x,u,flag,id,T)
  switch flag
   case 0,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys, x0, str, ts] = mdlInitializeSizes(id,T);
   case 1,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Derivatives
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 2,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % State Updates
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 3,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Calculate Outputs
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = rt_clock_update(t,id);;
   case { 4 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 9, %mdlTerminate
    sys = [];
   otherwise
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Unexpected flags %
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    error(['Unhandled flag = ', num2str(flag)]);
  end;

%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
%
function [sys, x0, str, ts] = mdlInitializeSizes(id,T)
  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0;
  sizes.NumOutputs     = 1;
  sizes.NumInputs      = 0;
  sizes.DirFeedthrough = 0;
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  rt_clock_init(id,T);
  x0=[];
  str = [];
  ts = [T 0]; % Fixed Step Discrete Time System

function rt_clock_init(id,T)
  global rt_timers;
  rt_timers.clock{id} = clock;
  rt_timers.T(id)=T;
  rt_timers.t(id)=0;
  rt_timers.i(id)=1;
  
function sys = rt_clock_update(t,id)
  global rt_timers;
  if ~t
      disp('re-initializing clock');
      sys = rt_timers.T(id);
      rt_timers.t(id)=t;
      rt_timers.clock{id}=clock;
      rt_timers.i(id)=1;
      return;
  end
  ctime = clock;
  dT = calc_dT(t,id,ctime);
  debug(sprintf('dT=%f,id=%d',dT,id));
  debug(sprintf('t=%f,id=%d',t,id));
  if dT < rt_timers.T(id)*rt_timers.i(id)
      p_t = rt_timers.T(id)*rt_timers.i(id) - dT;
      debug(sprintf('pause p_t=%f',p_t));
      pause(p_t);
  end
  rt_timers.i(id) = rt_timers.i(id)+1;
  sys = t + rt_timers.T(id);
  
function dT = calc_dT(t,id,ctime)
  global rt_timers;
  N_max=20;
  if t > rt_timers.t(id)
    dT = etime(ctime,rt_timers.clock{id});
    rt_timers.t(id)=t;
  else
    dT = rt_timers.T(id);
  end
  if rt_timers.i == N_max
      dT = max(0,dT - rt_timers.T(id)*(N_max-1));
      rt_timers.clock{id}=ctime;
      rt_timers.i = 1;
  end
  
function debug(msg)
  return;
  disp(msg);