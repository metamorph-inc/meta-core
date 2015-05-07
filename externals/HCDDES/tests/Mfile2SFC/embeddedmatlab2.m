double 4

function y = compute_f2(u)
% This block supports an embeddable subset of the MATLAB language.
% See the help menu for details. 
 
y = 0;
 
if (abs(u(1)) <  1.2*pi/2) && (abs(u(1)) >  0.8*pi/2)
    y = u(4);
else
    
    cos_angle = u(3)/cos(u(1));
    sin_angle = u(2)/cos(u(1));
    if (cos_angle >= 0)  %First Quadrant
        y = asin(sin_angle);
    end
    if (cos_angle < 0) %Second and Third Quadrant
        y = pi - asin(sin_angle);
    end
end
