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

% Author: Nicholas Kottenstette
% Description: Implements the averaging power junction 
%%%%%%%%%%%%%%%%%%%%%%%%%%
% S-Function Main
%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys, x0, str, ts] = avg_power_junction_m(t,x,u,flag,ms,np,nc,Ts)
  switch flag
   case 0,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys, x0, str, ts] = mdlInitializeSizes(ms,np,nc,Ts);
   case 1,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Derivatives
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 2,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % State Updates
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = mdlUpdate(t,x,u,ms,np,nc);
   case { 3 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Calculate Outputs
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = mdlOutputs(t,x,u,ms,np,nc);
   case 4,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % GetTimeOfNextVarHit
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    error(['Unhandled flag = ', num2str(flag)]);
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
function [sys,x0, str,ts] = mdlInitializeSizes(ms,np,nc,Ts)
    sizes = simsizes;
    sizes.NumContStates  = 0;
    sizes.NumOutputs     = (nc+np)*ms;
    sizes.NumDiscStates  = 0;
    sizes.NumInputs      = (nc+np)*(1+ms);
    sizes.DirFeedthrough = 1;   % must set this to true for the S-function to take the current inputs
    sizes.NumSampleTimes = 1;
    sys = simsizes(sizes);
    x0=[];
    str = [];
    ts = [Ts 0]; % Fixed Step Discrete sample time
  
%=============================================================================
% mdlUpdate
% Store the updated state if uav_index == ui
%=============================================================================
function  sys = mdlUpdate(t,x,u,ms,np,nc)
    sys = [];
    
%=============================================================================
% mdlTerminate
% Stop the simulation
%=============================================================================
function mdlTerminate()

%=============================================================================
% mdlOutputs
% Updates the model outputs
%=============================================================================
function sys=mdlOutputs(t,x,u,ms,np,nc)
    v_v=u(1:(nc*(ms+1)));
    u_v=u(nc*(ms+1)+1:(nc+np)*(ms+1));
    vp = calc_uv_pc(v_v,nc,np,ms);
    uc = calc_uv_pc(u_v,np,nc,ms);
    sys = zeros(size(u(1:(nc+np)*ms)));
    for i = 1:nc
        strt=ms*(i-1);
        sys((strt+1):strt+ms)=uc;
    end
    for i = 1:np
        strt=nc*ms + ms*(i-1); 
        sys((strt+1):strt+ms)=vp;
    end

function uv_pc = calc_uv_pc(uv_v,n_pc,nn_pc,ms)
    sz=ms+1;
    uv_s=0*uv_v(1:ms);
    uv_s2=uv_s;
    uv_as=uv_s;
    for i=1:n_pc
        strt=sz*(i-1);
        status_i=uv_v(1+strt);
        uv_i = uv_v(2+strt:2+strt+ms-1);
        uv_s = uv_s + status_i*uv_i;
        uv_as = uv_as + status_i*abs(uv_i);
        uv_s2 = uv_s2 + status_i*(uv_i.^2);
    end
    uv_sf = abs(uv_s)./(uv_as+1.0e-6);
    uv_pc = (1/sqrt(nn_pc))*sign(uv_s).*sqrt(uv_s2).*uv_sf;