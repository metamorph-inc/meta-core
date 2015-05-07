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
function [sys, x0, str, ts] = res_power_junction_m(t,x,u,flag,ms,mc,Ts)
  switch flag
   case 0,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Initialization
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    [sys, x0, str, ts] = mdlInitializeSizes(ms,mc,Ts);
   case 1,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Derivatives
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = [];
   case 2,
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % State Updates
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = mdlUpdate(t,x,u,ms,mc);
   case { 3 }
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    % Calculate Outputs
    %%%%%%%%%%%%%%%%%%%%%%%%%%
    sys = mdlOutputs(t,x,u,ms,mc);
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
function [sys,x0, str,ts] = mdlInitializeSizes(ms,mc,Ts)
    sizes = simsizes;
    sizes.NumContStates  = 0;
    sizes.NumOutputs     = (1+mc)*ms;
    sizes.NumInputs      = sizes.NumOutputs;
    sizes.NumDiscStates  = mc+sizes.NumOutputs;
    sizes.DirFeedthrough = 1; % must set this to true for the S-function to take the current inputs
    sizes.NumSampleTimes = 1;
    sys = simsizes(sizes);
    x0=zeros(1,sizes.NumDiscStates); %E(0)|uc(i)|vp(i)
    str = [];
    ts = [Ts 0]; % Fixed Step Discrete sample time
  
%=============================================================================
% mdlUpdate
% Store the updated state if uav_index == ui
%=============================================================================
function  sys = mdlUpdate(t,x,u,ms,mc)
    E_N_1=x(1:mc);
    vj_i=u(1:ms*mc);
    un_i=u(ms*mc+1:length(u));
    m_h = calc_m_h(E_N_1);
    u1_h = calc_u1(un_i,m_h);
    [E_N_h,m_N] = compute_E_N(E_N_1,u1_h,vj_i,m_h,ms)
    if m_N < m_h
        u1 = calc_u1(un_i,m_N);
        [E_N,m_N_i] = compute_E_N(E_N_1,u1,vj_i,m_N,ms);
        for i = m_N+1:mc
            E_N(i)=E_N_h(i);
        end
    else
        u1 = u1_h;
        E_N = E_N_h;
    end
    uc = rep_vec(u1,m_N);
    for i = m_N+1:mc
        uc = cat_vec(uc,vj_i(1+ms*(i-1):ms*i));
    end
    vp = calc_vn(m_N,vj_i,ms);
    sys = cat_vec(E_N,uc);
    sys = cat_vec(sys,vp);

%=============================================================================
% mdlOutputs
% Updates the model outputs
% [uc, vp]
% x = [E_N,uc,vp]
%=============================================================================
function sys=mdlOutputs(t,x,u,ms,mc)
    sys=x(1+mc:length(x));
    
function [E_N,m_N] = compute_E_N(E_N_1,u1,vj_i,m_h,ms)
    E_N = zeros(size(E_N_1));
    for i = 1:m_h
        vj = vj_i(1+(i-1)*ms:i*ms);
        E_N(i) = E_N_1(i) + dot(u1,u1) - dot(vj,vj);
    end
    m_N = calc_m_h(E_N);
    
function w_c = cat_vec(w1,w2)
    [m1,n1]=size(w1);
    [m2,n2]=size(w2);
    if m1 == 1
        if m2 == m1
            w_c = [w1,w2];
        else
            w_c = [w1,w2'];
        end
    else
        if m2 == m1
            w_c = [w1;w2];
        else
            w_c = [w1;w2'];
        end
    end
    
function w_r = rep_vec(w,n)
    [m_w,n_w]=size(w);
    if m_w==1
        mm=1;
        nn=n;
    else
        mm=n;
        nn=1;
    end
    w_r=repmat(w,mm,nn);
    
function vn = calc_vn(m,vj_i,ms)
    [sf_v,s_v,ss_v2] = calc_sf_sw_ssw2(m,vj_i,ms);
    vn = sf_v.*sign(s_v).*ss_v2;
    
function [sf,s_w,ss_w2] = calc_sf_sw_ssw2(n,wj,ms)
    % [sf,s_w,ss_w2] = calc_sf_sw_ssw2(n,wj,ms)
    % inputs:
    % n  - number of u or v vectors concat. in wj
    % wj - concat. vector or either u or v vectors
    % ms - length of each w vector
    % outputs:
    % sf  - scale factor vector
    % s_w - sum of w vectors
    % ss_w2 - square-root of sum of w-squared vectors
    s_w=zeros(size(wj(1:ms)));
    s_aw=1.0e-6*ones(size(s_w));
    ss_w2=zeros(size(s_w));
    for i = 1:n
        w = wj(1+(i-1)*ms:i*ms);
        s_w = s_w + w;
        s_aw = s_aw + abs(w);
        ss_w2 = ss_w2 + w.^2;
    end
    sf = abs(s_w)./s_aw;
    ss_w2 = sqrt(ss_w2);
    
function u1 = calc_u1(un_i,m)
    u1=(1/sqrt(m))*un_i;
    
function m_h = calc_m_h(Ev)
    I=find(Ev == Ev(1));
    m_h=max(I);
    m_h=length(Ev);
%=============================================================================
% mdlTerminate
% Stop the simulation
%=============================================================================
function mdlTerminate()

    
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