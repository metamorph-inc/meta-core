function [ K, a, H ] = cascade_integrators( Ts, N )
%[ K, a, H ] = cascade_integrators( Ts, N )
% Inputs close the loop with nested-positive feed-back
% for N integrators
% sampling rate Ts
% K is vector of gains
% a is corresponding cone-bound for closed-loop for Hn, Hn-1,...
% H is the final closed-loop plant
    kn = .5*pi/Ts;
    K = kn*ones(1,N);
    a = ones(size(K));
    H = series(K(1),tf(1,[1,0]));
    [Re,Im,W]=nyquist(H);
    a(1)=min(Re);
    H = feedback(H,1);
    for i = 1:N-1
        K(i+1) = K(i)/2;
        H = series(K(i+1)*H,tf(1,[1,0]));
        [Re,Im,W]=nyquist(H);
        a(i+1)=min(Re);
        if a(i+1) < -1
            disp('Look out may not be stable closed-loop')
            K(i+1) = -K(i+1)/(2*a(i+1));
            H = -H/(2*a(i+1));
            a(i+1) = -1/2;
        end
        H = feedback(H,1);
    end
    step(H);grid;
    
function Hp = se_ord(zeta,wn)
    Hp = tf(wn^2,[1,2*zeta*wn,wn^2]);