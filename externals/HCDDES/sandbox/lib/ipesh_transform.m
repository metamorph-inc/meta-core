function Hpz = ipesh_transform(H,Ts)
    %Hpz = ipesh_transform(H,Ts)
    %Inputs:
    %H continuous-time SISO LTI model to synthesize discrete-time filter from
    %Ts sampling rate
    %Outputs:
    %Hpz z-transform resulting from ipesh_transform
    %Author: Nicholas Kottenstette    
    Hs=series(H,tf(1,[1 0 0]));
    Hzs=c2d(Hs,Ts,'impulse');
    Hpz=minreal(series(tf([1 -2 1],Ts*[1 0],Ts),Hzs));
    %Hbz=c2d(H,Ts,'tustin')
    %figure(2);
    %bode(H,'g',Hpz,'bo-');
    