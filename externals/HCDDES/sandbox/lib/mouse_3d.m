%
% Copyright (c) 2008, Institute for Software Integrated Systems, Vanderbilt University
% All rights reserved.
%
% Permission to use, copy, modify, and distribute this software and its
% documentation for any purpose, without fee, and without written agreement is
% hereby granted, provided that the above copyright notice, the following
% two paragraphs and the author appear in all copies of this software.
%
% IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
% DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
% OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
% UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
%
% THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
% INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
% AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
% ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
% PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
%

%%%%%%%%%%%%%%%%%%%%%%%%%%
% S-Function Main
%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys, x0, str, ts] = mouse_3d(t,x,u,flag)
  persistent srv;
  switch flag
   case { 0 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys,x0, str, ts, srv] = mdlInitializeSizes(srv);
   case { 1 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Derivatives
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case { 2 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % State Updates
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case { 3 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Calculate Outputs
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys=mdlOutputs(srv);
   case { 4 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % sys = mdlGetTimeOfNextVarHit(t,delta_t);
    sys = [];
   case { 9 }
    srv=mdlTerminate(srv);
   otherwise
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Unexpected flags %
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    error(['Unhandled flag = ', num2str(flag)]);
  end;
%%%%%%%%%%%%%%%%%%%%%%%%%% end S-Function Main



%
%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
function [sys,x0, str, ts, srv] = mdlInitializeSizes(srv)
  if isempty(srv)
    srv = actxserver('TDxInput.Device');
  end
  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0;
  sizes.NumOutputs     = 6 + srv.Keyboard.Keys;
  sizes.NumInputs      = 0;
  sizes.DirFeedthrough = 0;   % must set this to true for the S-function to take the current inputs
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  x0=[];
  str = [];
  ts = [0.01 0]; % Fixed step discrete time
  
%=============================================================================
% mdlTerminate
% Stop the simulation
%=============================================================================
function srv=mdlTerminate(srv)
  delete(srv);
  srv=[];

%=============================================================================
% mdlGetTimeOfNextVarHit
% Calculate when Simulink needs to update the current state of this block
%=============================================================================
%
%function sys = mdlGetTimeOfNextVarHit(t,delta_t)
%    sys = t+delta_t;
    
%=============================================================================
% mdlOutputs
% Calculate outputs (= interaction received)
%=============================================================================
function sys=mdlOutputs(srv)
  pos = [srv.Sensor.Translation.X() srv.Sensor.Translation.Y() srv.Sensor.Translation.Z()];
  orot = srv.get('Sensor').get('Rotation');
  rot = [srv.Sensor.Rotation.X() srv.Sensor.Rotation.Y() srv.Sensor.Rotation.Z()];
  sys = [pos, rot];
  for i=1:srv.Keyboard.Keys
    sys(6+i)=srv.Keyboard.IsKeyDown(i);
  end
