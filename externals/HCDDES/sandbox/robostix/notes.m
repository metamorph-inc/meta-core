%How to implement a passive, lead compensator in discrete time
%vectorized :-D
persistent x_1;
persistent y_1;
persistent a_1;
persistent b_1;
persistent b_2;
if isempty(x_1)
    x_1 = zeros(4,1,'single');
    y_1 = zeros(4,1,'single');
    n=single([3.2243128e4, -2.9237296e4]);
    d=single([3.1415926e3, -1.3576052e2]);
    a_1=single(-d(2)/d(1));
    b_1=single(n(2)/d(1));
    b_2=single(n(1)/d(1));
end
y = a_1*y_1 + b_1*x_1 + b_2*x;
y_1 = y;
x_1 = x;