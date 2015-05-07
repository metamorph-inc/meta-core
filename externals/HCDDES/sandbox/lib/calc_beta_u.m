% Author: Nicholas Kottenstette
% Description: Calculates diag(beta(u)) components
%              inputs are [G(s(u))',s(u)',u']' 
%%%%%%%%%%%%%%%%%%%%%%%%%%
% S-Function Main
%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys, x0, str, ts] = calc_beta_u(t,x,u,flag,n)
  switch flag
   case 0,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys, x0, str, ts] = mdlInitializeSizes(n);
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
    sys = mdlOutputs(t,x,u);
   case 4,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit (only for t = [-2 0], variable discrete
    % time step option) 
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 9,
    sys = [];
    mdlTerminate();
   otherwise
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Unexpected flags %
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    error(['Unhandled flag = ', num2str(flag)]);
  end
%%%%%%`%%%%%%%%%%%%%%%%%%%% end S-Function Main

%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
function [sys,x0, str,ts] = mdlInitializeSizes(n)
  sizes = simsizes;
  sizes.NumContStates  = 0;   %Don't need to track states
  sizes.NumDiscStates  = 0;   %Don't need to track states
  sizes.NumOutputs     = n;  %Based on inputs at runtime
  sizes.NumInputs      = 3*n;  %Figure it out at runtime
  sizes.DirFeedthrough = 1;   %Output depends directly on inputs
  sizes.NumSampleTimes = 1;   %Time (t) is a scalar
  sys = simsizes(sizes);
  x0=zeros(1,sizes.NumDiscStates);
  str = [];
  ts = [-1 0]; % Continuous time update
  
%=============================================================================
% mdlUpdate
% Store the updated state if uav_index == ui
%=============================================================================
function  sys = mdlUpdate(t,x,u,uav_index)
  
%=============================================================================
% mdlTerminate
% Stop the simulation
%=============================================================================
function mdlTerminate()

%=============================================================================
% mdlGetTimeOfNextVarHit
% Calculate when Simulink needs to update the current state of this block
%=============================================================================
function sys = mdlGetTimeOfNextVarHit(t)

%=============================================================================
% mdlOutputs
% Updates the model outputs
%=============================================================================
function betau_v = mdlOutputs(t,x,u)
  n=length(u)/3;
  Gsu=u(1:n);
  su=u(n+1:2*n);
  uv=u(2*n+1:3*n);
  betau_v=su./uv;
  betau_v(find(uv==0))=1;
