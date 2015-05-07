function Vi = find_vi(vh,vinf,alpha)
  count = 1;
  vi = vh;
  rhs = vh^2/(sqrt((vinf*cos(alpha))^2+(vinf*sin(alpha)+vi)^2));
  vimax = 10;
  while ((abs(vi-rhs) > 0.0001) || (count < 50))
    if (vi > rhs)
      dv = -vimax/2^count;
    else
      dv = vimax/2^count;
    end
    count = count+1;
    vi = max(0,vi + dv);
    rhs = vh^2/(sqrt((vinf*cos(alpha))^2+(vinf*sin(alpha)+vi)^2));
  end
  Vi = vi;
  