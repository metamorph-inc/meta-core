function Hpz = ipesh_transform(H,Ts)
    Hs=series(H,tf(1,[1 0 0]));
    Hzs=c2d(Hs,Ts,'impulse');
    Hpz=minreal(series(tf([1 -2 1],Ts*[1 0],Ts),Hzs));
    Hbz=c2d(H,Ts,'tustin')
    %Hbz=c2d(H,Ts,'prewarp', 40)
    bode(H,'g',Hpz,'bo-',Hbz,'r^-');
    