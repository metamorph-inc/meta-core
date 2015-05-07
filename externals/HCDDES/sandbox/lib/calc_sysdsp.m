function [x0,sysdsp] = calc_sysdsp(Kp,Kd,Kc,x0,Ts)
%sysdsp = calc_sysdsp(Kp,Kd,Kx,x0,Ts)
  tst=false;
  if tst
    Ao=[zeros(size(Kp)), zeros(size(Kp));Kp, zeros(size(Kp))];
    Bo=[eye(size(Kp)); Kd];
    Co=[zeros(size(Kp)) eye(size(Kp))];
    Do=zeros(size(Kp));
    [m,n]=size(x0);
    [am,an]=size(Kp);
    if (an == n) && (am == 1)
      x0 = [x0; zeros(size(x0))];
    elseif (am == n) && (an == 1)
      x0 = [x0, zeros(size(x0))]';
    else
      x0=0;
    end
    sysc=ss(Ao,Bo,Co,Do);
    if Ts <= 0
      Ts = 1;
    end
    sysd=c2d(sysc, Ts, 'zoh');
    Cp=sysd.c*(sysd.a - eye(size(sysd.a)));
    Dp=sysd.c*sysd.b;
    sysdp_old=ss(sysd.a, sysd.b, Cp, Dp, Ts);
    if size(Kc) ~= size(sysdp_old.d)
      if size(Kc) == [1,1]
        Kc = Kc*eye(size(sysdp_old.d));
      else
        Kc = max(max(Kc))*eye(size(sysdp_old.d));
      end
    end
    sysdsp_old=feedback(sysdp_old,Kc);
  end
  sysdp.a=eye(size(Kp));
  sysdp.b=Ts*eye(size(Kp));
  sysdp.c=Ts*Kp;
  sysdp.d=0.5*(Ts^2)*Kp + Ts*Kd;
  if tst
    tf_dp=tf(ss(sysdp.a,sysdp.b,sysdp.c,sysdp.d,Ts))
    tf_dp_old=tf(sysdp_old)
    diff_tf=tf_dp-tf_dp_old
  end
  if size(Kc) ~= size(sysdp.d)
    if size(Kc) == [1,1]
      Kc = Kc*eye(size(sysdp.d));
    else
      Kc = max(max(Kc))*eye(size(sysdp.d));
    end
  end
  if tst
    G=eye(size(sysdp.d))+Kc*sysdp.d;
    Gi=inv(G);
    sysdsp.a=sysdp.a - Kc*sysdp.b*Gi*sysdp.c;
    sysdsp.b=sysdp.b*(eye(size(sysdp.d)) - Kc*Gi*sysdp.d);
    sysdsp.c=Gi*sysdp.c;
    sysdsp.d=Gi*sysdp.d;
    sysdsp = ss(sysdsp.a,sysdsp.b,sysdsp.c,sysdsp.d,Ts);
    tf_dsp=tf(sysdsp)
    tf_dsp_old=tf(sysdsp_old)
    diff_tf=tf_dsp-tf_dsp_old
  else
    sysdp=ss(sysdp.a,sysdp.b,sysdp.c,sysdp.d,Ts);
    sysdsp=feedback(sysdp,Kc);
  end
  