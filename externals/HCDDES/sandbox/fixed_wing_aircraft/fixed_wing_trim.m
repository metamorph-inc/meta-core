function [F_T_v,alpha_v,delta_e_v,Sol] = fixed_wing_trim(CD,CL,Cm,S_v,V_v,h_v,m_v,delta_f_v,Sol,g_f)
    %[F_T,alpha,delta_e,Sol] = fixed_wing_trim(CD,CL,Cm,S_v,V_v,h_v,m_v,delta_f_v,Sol,g_f)
    %Calculate resulting:
    %F_T [N]           (engine-thrust)
    %alpha [radians]   (angle-of-attack)
    %delta_e [radians] (elevator-angle)
    %Sol (trim-solution)
    %Given:
    %CD = [CDo,CDa,CDq,CDde,CDin] - drag coefficents for CDs
    %-CDs = CDo+ CDin*delta_r + CDq*c_bar*q/(2V) + CDa*alpha + CDde*delta_e
    %CL = [CLo,CLa,CLq,CLde,CLin] - lift coefficients for CL
    %-CL = CLo+ CLin*delta_r + CLq*c_bar*q/(2V) + CLa*alpha + CLde*delta_e
    %Cm = [Cmo,Cma,Cmq,Cmde,Cmin] - m-moment coefficients for Cm
    %-Cm = Cmo+ Cmin*delta_r + Cmq*c_bar*q/(2V) + Cma*alpha + Cmde*delta_e
    %S_v [reference surface area value] (m^2)
    %V_v [aircraft-velocity value, gamma=0, chi=0] (m/s)
    %h_v [aircraft-altitude value] (m)
    %m_v [aircraft mass value] (kg)
    %delta_f_v [incidence angle of stabilator] (radians)
    %g_f [generate plot flag] false or true
    %Example:
    %CD=[0.048,0.384,0,0,0];CL=[0.2,5.15,4.1,0.5,0];
    %Cm=[0.025,-0.7,-14.9,-1.12,0];S_v=(.3048^2)*182;
    %V_v=100;h_v=1e3;m_v=.4535959237*6360;
    %[F_T,alpha,delta_e,Sol]=fixed_wing_trim(CD,CL,Cm,S_v,V_v,h_v,m_v,0,[],true);
    syms q_bar S m g alpha CDo CD_in CDa CDde CLo CL_in CLa CLde Cmo Cm_in Cma Cmde F_T delta_f delta_e;
    eq_1 = 'F_T - q_bar*S*(CDa*alpha + CDo + CD_in*delta_f + CDde*delta_e)';
    eq_2 = '-F_T*alpha - q_bar*S*(CLa*alpha + CLo + CL_in*delta_f + CLde*delta_e) + m*g';
    eq_3 = 'Cmde*delta_e + Cma*alpha + Cmo + Cm_in*delta_f';
    if isempty(Sol)
        Sol = solve(eq_1,eq_2,eq_3,'F_T,alpha,delta_e');
    end
    [rho_v,g_v] = calc_rho_g(h_v);
    q_bar_v = (1/2)*rho_v*V_v^2;
    var_list = {S, m, g, CDo, CDa, CDde, CD_in, CLo, CLa, CLde, CL_in, Cmo, Cma, Cmde, Cm_in, q_bar, delta_f };
    var_vals = {S_v, m_v, g_v, CD(1),CD(2),CD(4),CD(5),CL(1),CL(2),CL(4),CL(5),Cm(1),Cm(2),Cm(4),Cm(5),q_bar_v,delta_f_v};
    F_T_v = subs(Sol.F_T,var_list,var_vals);
    alpha_v = subs(Sol.alpha,var_list,var_vals);
    delta_e_v = subs(Sol.delta_e,var_list,var_vals);
    F_T_v = F_T_v(1);
    alpha_v = alpha_v(1);
    delta_e_v = delta_e_v(1);
    if g_f
        V_a = .5*V_v:(2-.5)*V_v/19:2*V_v;
        alpha_a = zeros(size(V_a));
        delta_e_a = zeros(size(V_a));
        F_T_a = zeros(size(V_a));
        var_list_p = {S, m, g, CDo, CDa, CDde, CD_in, CLo, CLa, CLde, CL_in, Cmo, Cma, Cmde, Cm_in, delta_f };
        var_vals_p = {S_v, m_v, g_v, CD(1),CD(2),CD(4),CD(5),CL(1),CL(2),CL(4),CL(5),Cm(1),Cm(2),Cm(4),Cm(5), delta_f_v};
        S_F_T = subs(Sol.F_T,var_list_p,var_vals_p);
        S_alpha = subs(Sol.alpha,var_list_p,var_vals_p);
        S_delta_e = subs(Sol.delta_e,var_list_p,var_vals_p);
        for i = 1:length(V_a)
            disp(['i = ',num2str(i)]);
            q_bar_v = (1/2)*rho_v*V_a(i)^2;
            t=subs(S_F_T,q_bar,q_bar_v);
            F_T_a(i)=t(1);
            t=subs(S_alpha,q_bar,q_bar_v);
            alpha_a(i)=t(1);
            t=subs(S_delta_e,q_bar,q_bar_v);
            delta_e_a(i)=t(1);
        end
        [AX,H1,H2]=plotyy(V_a,[alpha_a;delta_e_a]*180/pi,V_a,F_T_a*1e-3);grid;
        set(get(AX(1),'Ylabel'),'String','Degrees');
        set(get(AX(2),'Ylabel'),'String','F_T (kN)');
        get( H1(1) )
        set( H1(1), 'DisplayName', '\alpha');
        set( H1(2), 'DisplayName', '\delta_e');
        set( H2, 'DisplayName', 'F_T');
        xlabel('Velocity (m/s)');
    end
    
        
function [rho_v,g_v] = calc_rho_g(h_v)
    %h altitude (meters)
    %g gravity at h (m/s^2)
    g_v = 9.80665*((6356e3/(6356e3+h_v))^2);
    R = 8.31432; %J/(mol-Kelvin)
    M = 28.9644; %gm/mol
    To = 288.15; %standard temperature [Kelvin]
    Po = 101325; %standard pressure [Pa]
    L = 6.5; %Kelvin/km
    %gamma = 1.4; %Gas constant for air
    T = To - 1.0e-3*L*h_v;            %Temperature at h
    ps = Po*(T/To)^((g_v*M)/(R*L));   %Pressure at h
    rho_v = ps/((1000*R/M)*T);        %Density at h
    %a = sqrt(gamma*(R/(M*1e-3))*T); %speed of sound (m/s)