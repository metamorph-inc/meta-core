%%%%%%%%%%%%%%%%%%%%%%%%%%
% S-Function Main
%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys, x0, str, ts] = rt_clock(t,x,u,flag,id,T)
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
    sys = [];
   case { 4 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = rt_clock_update(t,id);
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
  sizes.NumOutputs     = 0;
  sizes.NumInputs      = 0;
  sizes.DirFeedthrough = 0;
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  rt_clock_init(id,T);
  x0=[];
  str = [];
  ts = [-2 0]; % Variable Step Discrete Time System

function rt_clock_init(id,T)
  global rt_timers;
  rt_timers.clock{id} = clock;
  rt_timers.T(id)=T;
  rt_timers.t(id)=0;
  
function sys = rt_clock_update(t,id)
  global rt_timers;
  if ~t
      sys = rt_timers.T(id);
      rt_timers.t(id)=t;
      rt_timers.clock{id}=clock;
      return;
  end
  ctime = clock;
  dT = calc_dT(t,id,ctime);
  debug(sprintf('dT=%f,id=%d',dT,id));
  debug(sprintf('t=%f,id=%d',t,id));
  if dT < rt_timers.T(id)
      p_t = rt_timers.T(id) - dT;
      debug(sprintf('pause p_t=%f',p_t));
      pause(p_t);
  end
  sys = t + rt_timers.T(id);
  
function dT = calc_dT(t,id,ctime)
  global rt_timers;
  if t > rt_timers.t(id)
    dT = etime(ctime,rt_timers.clock{id});
    rt_timers.clock{id}=ctime;
    rt_timers.t(id)=t;
  else
    dT = rt_timers.T(id);
  end
  
function debug(msg)
  return;
  disp(msg);