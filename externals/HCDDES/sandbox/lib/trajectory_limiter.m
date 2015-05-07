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
function [sys, x0, str, ts] = trajectory_limiter(t,x,u,flag,xyz_0,dxyz_max,delta_t,scale_factor)
  global p_wpf; %way point fifo
  global p_lwp; %last waypoint
  global p_cwp; %current waypoint
  switch flag
   case { 0 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys,x0, str, ts] = mdlInitializeSizes(delta_t/2);
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
    sys=mdlOutputs(t,x,u,xyz_0,dxyz_max,scale_factor);
   case { 4 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    error(['Unhandled flag = ', num2str(flag)]);
   case { 9 }
    p_wpf=[];
    p_lpw=[];
    p_cwp=[];
    sys = [];
    mdlTerminate();
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
function [sys,x0, str,ts] = mdlInitializeSizes(delta_t)
  global p_wpf; %way point fifo
  global p_lwp; %last waypoint
  global p_cwp; %current waypoint
  p_wpf=[];
  p_lwp=[];
  p_cwp=[];
  sizes = simsizes;
  sizes.NumContStates  = 0;
  sizes.NumDiscStates  = 0;
  sizes.NumOutputs     = 4;
  sizes.NumInputs      = 5;
  sizes.DirFeedthrough = 1;   % must set this to true for the S-function to take the current inputs
  sizes.NumSampleTimes = 1;
  sys = simsizes(sizes);
  x0=[];
  str = [];
  ts = [delta_t 0]; % Fixed Step Discrete sample time
  
%=============================================================================
% mdlTerminate
% Stop the simulation
%=============================================================================
function mdlTerminate()

%=============================================================================
% mdlOutputs
% Calculate outputs (= interaction received)
%=============================================================================
function sys=mdlOutputs(t,x,u,xyz_0,dxyz_max,scale_factor)
  persistent last_out;
  % need a variable to decide if we are waiting or flying
  i_s_f=(1/scale_factor);
  if ~t
    %xyz_0 = xyz_0*i_s_f;
    last_out = struct('x',0,'y',0,'z',0,'yaw',0); %vehicles always start at zero
  end
  %determine current way point, last way point and flight mode
  [cwp,lwp,fmd] = get_cur_last_waypoints(t,last_out,u);
  cwp.x = cwp.x - xyz_0(1); %XXX We assume user is unaware of our hack
  cwp.y = cwp.y - xyz_0(2);
  cwp.z = cwp.z - xyz_0(3);
  dx = cwp.x*i_s_f - last_out.x;
  dy = cwp.y*i_s_f - last_out.y;
  dz = cwp.z*i_s_f - last_out.z;
  last_out.x = last_out.x + sign(dx)*min(abs(dx),dxyz_max(1));
  last_out.y = last_out.y + sign(dy)*min(abs(dy),dxyz_max(2));
  last_out.z = last_out.z + sign(dz)*min(abs(dz),dxyz_max(3));
  last_out.yaw = cwp.yaw;
  sys = [last_out.x last_out.y last_out.z last_out.yaw];
  
%=============================================================================
% wp = get_waypoint(u)
% returns structure containing waypoint in u
%=============================================================================
function wp = get_waypoint(u)
  wp.t = u(1);
  wp.x = u(2);
  wp.y = u(3);
  wp.z = u(4);
  wp.yaw = u(5);
  
%=============================================================================
% [cwp,lwp,fmd] = get_cur_last_waypoint(t,hc,u)
% returns current way point (cwp), last waypoint (lwp), and flight
% mode (fmd)
% given time (t), current heading (hc), and mdl input (u)
%=============================================================================
function [cwp,lwp,fmd] = get_cur_last_waypoints(t,hc,u)
  global p_wpf; %way point fifo
  global p_lwp; %last waypoint is the last waypoint which
                %we replaced when updating cwp
  global p_cwp; %current waypoint is the waypoint in which
                %t < cwp.t and we need to reach it
  wpf=p_wpf;
  lwp=p_lwp;
  cwp=p_cwp;
  if (isempty(wpf))
    N=128;
    dim=[1,4];
    wpf = fifo([],[],[N dim]);
  end
  if (isempty(lwp))
    lwp=struct('t',t,'x',hc.x,'y',hc.y,'z',hc.z,'yaw',hc.yaw);
  end
  if (isempty(cwp))
    cwp = lwp;
  end
  wpc=get_waypoint(u);
  dt=max(0,(t-1));
  if (wpc.t > dt) && (wpc.t > cwp.t)
    [wpf,v]=fifo(wpf.PUSH,wpf,wpc); %add new waypoint to fifo
  end
  fmd=true;
  if (cwp.t < t)
    %time to get a new current way point
    while true
      [wpf,wp]=fifo(wpf.POP,wpf,[]);
      if ~isempty(wp)
        lwp=cwp;  %XXX this is how it should update lwp
        cwp=wp;    %assign current way point from fifo
      else
        fmd=false; %current wap point expired, but no new way point
                   %to go to
        break;     %fifo empty exit out
      end
      if (cwp.t >= t)
        break;
      else
        %keep looping for 'fresh' way point
      end
    end
  end
  %Update appropriate arrays
  p_wpf=wpf;
  p_lwp=lwp;
  p_cwp=cwp;
  