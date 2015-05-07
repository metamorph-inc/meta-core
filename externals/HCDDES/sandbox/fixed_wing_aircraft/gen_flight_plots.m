function gen_flight_plots( V_s, gamma_s, chi_s, x2_s, omega_s, actuator_s )
%gen_flight_plots(V_s, gamma_s, chi_s, x2_s, omega_s, actuator_s )
    [t,V_set,V,V_e] = parse_struct(V_s);
    figure(1);
    plot(t,V_set);grid;xlabel('t (s)');ylabel('V_s (m/s)');
    figure(2);
    F_T=actuator_s.signals(1).values;
    [AX,H1,H2]=plotyy(t,V_e,t,F_T);xlabel('t (s)');
    ylabel('(V_s-V) (m/s)');
    set(get(AX(2),'Ylabel'),'String','F_T (N)');
    [t,gamma_set,gamma,gamma_e] = parse_struct(gamma_s);
    [t,chi_set,chi,chi_e]=parse_struct(chi_s);
    figure(3);
    [AX,H1,H2]=plotyy(t,(180/pi)*gamma_set,t, (180/pi)*chi_set);grid;xlabel('t (s)');
    ylabel('\gamma_s(deg)');
    set(get(AX(2),'Ylabel'),'String','\chi_s (deg)');
    figure(4);
    [AX,H1,H2]=plotyy(t,(180/pi)*gamma_e,t, (180/pi)*chi_e);grid;xlabel('t (s)');
    ylabel('(\gamma_s-\gamma)(deg)');
    set(get(AX(2),'Ylabel'),'String','(\chi_s-\chi) (deg)');
    [t,x2_set,x2,x2_e]=parse_struct(x2_s);
    figure(5);
    [AX,H1,H2]=plotyy(t,(180/pi)*x2(:,1),t,(180/pi)*x2(:,2:3));grid;xlabel('t (s)');ylabel('\mu (deg)');
    set(get(AX(2),'Ylabel'),'String','[\alpha,\beta](deg)');
    [t,omega_set,omega,omega_e]=parse_struct(omega_s);
    figure(6);
    [AX,H1,H2]=plotyy(t,(180/pi)*omega(:,1),t,(180/pi)*omega(:,2:3));grid;xlabel('t (s)');ylabel('p (deg/s)');
    set(get(AX(2),'Ylabel'),'String','[q,r](deg/s)');
end

function [t,set,out,e] = parse_struct(s)
    t = s.time;
    set = s.signals(1).values;
    out = s.signals(2).values;
    e = set-out;
end

