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
  